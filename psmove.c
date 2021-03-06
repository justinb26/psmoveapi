
 /**
 * PS Move API - An interface for the PS Move Motion Controller
 * Copyright (c) 2011, 2012 Thomas Perl <m@thp.io>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *    2. Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 **/

#include "psmove.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <unistd.h>

/* OS-specific includes, for getting the Bluetooth address */
#ifdef __APPLE__
#  include <IOBluetooth/Bluetooth.h>
#endif

#ifdef __linux
#  include <bluetooth/bluetooth.h>
#  include <bluetooth/hci.h>
#  include <sys/ioctl.h>
#endif

#ifdef _WIN32
#  include <windows.h>
#  include <Bthsdpdef.h>
#  include <BluetoothAPIs.h>
#endif

#include "moved_client.h"
#include "hidapi.h"


/* Begin private definitions */

/* Vendor ID and Product ID of PS Move Controller */
#define PSMOVE_VID 0x054c
#define PSMOVE_PID 0x03d5

/* Buffer size for writing LEDs and reading sensor data */
#define PSMOVE_BUFFER_SIZE 49

/* Buffer size for calibration data */
#define PSMOVE_CALIBRATION_SIZE 49

/* Buffer size for the Bluetooth address get request */
#define PSMOVE_BTADDR_GET_SIZE 16

/* Buffer size for the Bluetooth address set request */
#define PSMOVE_BTADDR_SET_SIZE 23

/* Maximum length of the serial string */
#define PSMOVE_MAX_SERIAL_LENGTH 255

enum PSMove_Request_Type {
    PSMove_Req_GetInput = 0x01,
    PSMove_Req_SetLEDs = 0x02,
    PSMove_Req_GetBTAddr = 0x04,
    PSMove_Req_SetBTAddr = 0x05,
    PSMove_Req_GetCalibration = 0x10,
};

enum PSMove_Device_Type {
    PSMove_HIDAPI = 0x01,
    PSMove_MOVED = 0x02,
};

typedef struct {
    unsigned char type; /* message type, must be PSMove_Req_SetLEDs */
    unsigned char _zero; /* must be zero */
    unsigned char r; /* red value, 0x00..0xff */
    unsigned char g; /* green value, 0x00..0xff */
    unsigned char b; /* blue value, 0x00..0xff */
    unsigned char rumble2; /* unknown, should be 0x00 for now */
    unsigned char rumble; /* rumble value, 0x00..0xff */
    unsigned char _padding[PSMOVE_BUFFER_SIZE-7]; /* must be zero */
} PSMove_Data_LEDs;

/* Decode 12-bit signed value (assuming two's complement) */
#define TWELVE_BIT_SIGNED(x) (((x) & 0x800)?(-(((~(x)) & 0xFFF) + 1)):(x))

typedef struct {
    unsigned char type; /* message type, must be PSMove_Req_GetInput */
    unsigned char buttons1;
    unsigned char buttons2;
    unsigned char buttons3;
    unsigned char buttons4;
    unsigned char trigger; /* trigger value; 0..255 */
    unsigned char trigger2; /* trigger value, 2nd frame */
    unsigned char _unk7;
    unsigned char _unk8;
    unsigned char _unk9;
    unsigned char _unk10;
    unsigned char timehigh; /* high byte of timestamp */
    unsigned char battery; /* battery level; 0x05 = max, 0xEE = USB charging */
    unsigned char aXlow; /* low byte of accelerometer X value */
    unsigned char aXhigh; /* high byte of accelerometer X value */
    unsigned char aYlow;
    unsigned char aYhigh;
    unsigned char aZlow;
    unsigned char aZhigh;
    unsigned char aXlow2; /* low byte of accelerometer X value, 2nd frame */
    unsigned char aXhigh2; /* high byte of accelerometer X value, 2nd frame */
    unsigned char aYlow2;
    unsigned char aYhigh2;
    unsigned char aZlow2;
    unsigned char aZhigh2;
    unsigned char gXlow; /* low byte of gyro X value */
    unsigned char gXhigh; /* high byte of gyro X value */
    unsigned char gYlow;
    unsigned char gYhigh;
    unsigned char gZlow;
    unsigned char gZhigh;
    unsigned char gXlow2; /* low byte of gyro X value, 2nd frame */
    unsigned char gXhigh2; /* high byte of gyro X value, 2nd frame */
    unsigned char gYlow2;
    unsigned char gYhigh2;
    unsigned char gZlow2;
    unsigned char gZhigh2;
    unsigned char temphigh; /* temperature (bits 12-5) */
    unsigned char templow_mXhigh; /* temp (bits 4-1); magneto X (bits 12-9) */
    unsigned char mXlow; /* magnetometer X (bits 8-1) */
    unsigned char mYhigh; /* magnetometer Y (bits 12-5) */
    unsigned char mYlow_mZhigh; /* magnetometer: Y (bits 4-1), Z (bits 12-9) */
    unsigned char mZlow; /* magnetometer Z (bits 8-1) */
    unsigned char timelow; /* low byte of timestamp */

    unsigned char _padding[PSMOVE_BUFFER_SIZE-44]; /* unknown */
} PSMove_Data_Input;

struct _PSMove {
    /* Device type (hidapi-based or moved-based */
    enum PSMove_Device_Type type;

    /* The handle to the HIDAPI device */
    hid_device *handle;

    /* The handle to the moved client */
    moved_client *client;
    int remote_id;

    /* Index (at connection time) - not exposed yet */
    int id;

    /* Various buffers for PS Move-related data */
    PSMove_Data_LEDs leds;
    PSMove_Data_Input input;

    /* Save location for the controller BTAddr */
    PSMove_Data_BTAddr btaddr;

    /* Save location for the serial number */
    char *serial_number;

#ifdef _WIN32
    int is_bluetooth;
#endif
};

/* Macro: Print a critical message if an assertion fails */
#define psmove_CRITICAL(x) \
        {fprintf(stderr, "[PSMOVE] Assertion fail in %s: %s\n", __func__, x);}

/* Macros: Return immediately if an assertion fails + log */
#define psmove_return_if_fail(expr) \
        {if(!(expr)){psmove_CRITICAL(#expr);return;}}
#define psmove_return_val_if_fail(expr, val) \
        {if(!(expr)){psmove_CRITICAL(#expr);return(val);}}

/* End private definitions */

static moved_client_list *clients;
static int psmove_local_disabled = 0;
static int psmove_remote_disabled = 0;

/* Number of valid, open PSMove* handles "in the wild" */
static int psmove_num_open_handles = 0;

/* Private functionality needed by the Linux version */
#if defined(__linux)

int _psmove_linux_bt_dev_info(int s, int dev_id, long arg)
{
    struct hci_dev_info di = { dev_id: dev_id };
    unsigned char *btaddr = (void*)arg;
    int i;

    if (ioctl(s, HCIGETDEVINFO, (void *) &di) == 0) {
        for (i=0; i<6; i++) {
            btaddr[i] = di.bdaddr.b[5-i];
        }
    }

    return 0;
}

#endif /* defined(__linux) */


/* Start implementation of the API */


void
psmove_disable_remote()
{
    psmove_remote_disabled = 1;
}

void
psmove_disable_local()
{
    psmove_local_disabled = 1;
}

void
_psmove_write_data(PSMove *move, unsigned char *data, int length)
{
    memcpy(&(move->leds), data, length);
    psmove_update_leds(move);
}

void
_psmove_read_data(PSMove *move, unsigned char *data, int length)
{
    assert(data != NULL);
    assert(length >= (sizeof(move->input) + 1));

    data[0] = psmove_poll(move);
    memcpy(data+1, &(move->input), sizeof(move->input));
}

int
psmove_is_remote(PSMove *move)
{
    return move->type == PSMove_MOVED;
}

void
psmove_reinit()
{
    if (psmove_num_open_handles != 0) {
        psmove_CRITICAL("reinit called with open handles "
                "(forgot psmove_disconnect?)");
        exit(0);
    }

    if (clients != NULL) {
        moved_client_list_destroy(clients);
        clients = NULL;
    }

    hid_exit();
}

int
psmove_count_connected_hidapi()
{
    struct hid_device_info *devs, *cur_dev;
    int count = 0;

    if (psmove_local_disabled) {
        return 0;
    }

    devs = hid_enumerate(PSMOVE_VID, PSMOVE_PID);
    cur_dev = devs;
    while (cur_dev) {
#ifdef _WIN32
        /**
         * Windows Quirk: Ignore extraneous devices (each dev is enumerated
         * 3 times, count only the one with "&col02#" in the path)
         *
         * We use col02 for enumeration, and col01 for connecting. We want to
         * have col02 here, because after connecting to col01, it disappears.
         **/
        if (strstr(cur_dev->path, "&col02#") == NULL) {
            count--;
        }
#endif
        count++;
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    return count;
}

int
psmove_count_connected_moved(moved_client *client)
{
    psmove_return_val_if_fail(client != NULL, 0);
    return moved_client_send(client, MOVED_REQ_COUNT_CONNECTED, 0, NULL);
}

int
psmove_count_connected()
{
    int count = psmove_count_connected_hidapi();

    if (clients == NULL && !psmove_remote_disabled) {
        clients = moved_client_list_open();
    }

    moved_client_list *cur;
    for (cur=clients; cur != NULL; cur=cur->next) {
        count += psmove_count_connected_moved(cur->client);
    }

    return count;
}

PSMove *
psmove_connect_internal(wchar_t *serial, char *path, int id)
{
    PSMove *move = (PSMove*)calloc(1, sizeof(PSMove));
    move->type = PSMove_HIDAPI;

#ifdef _WIN32
    /* Windows Quirk: USB devices have "0" as serial, BT devices their addr */
    if (wcslen(serial) > 1) {
        move->is_bluetooth = 1;
    }
    /* Windows Quirk: Use path instead of serial number by ignoring serial */
    serial = NULL;

    /* XXX Ugly: Convert "col02" path to "col01" path (tested w/ BT and USB) */
    char *p;
    psmove_return_val_if_fail((p = strstr(path, "&col02#")) != NULL, NULL);
    p[5] = '1';
    psmove_return_val_if_fail((p = strstr(path, "&0001#")) != NULL, NULL);
    p[4] = '0';
#endif

    if (serial == NULL && path != NULL) {
        move->handle = hid_open_path(path);
    } else {
        move->handle = hid_open(PSMOVE_VID, PSMOVE_PID, serial);
    }

    if (!move->handle) {
        free(move);
        return NULL;
    }

    /* Use Non-Blocking I/O */
    hid_set_nonblocking(move->handle, 1);

    /* Message type for LED set requests */
    move->leds.type = PSMove_Req_SetLEDs;

    /* Remember the ID/index */
    move->id = id;

    /* Remember the serial number */
    move->serial_number = (char*)calloc(PSMOVE_MAX_SERIAL_LENGTH, sizeof(char));
    if (serial != NULL) {
        wcstombs(move->serial_number, serial, PSMOVE_MAX_SERIAL_LENGTH);
    } else {
        // FIXME
        fprintf(stderr, "[PSMOVE] no serial, using path: %s\n", path);
        strcpy(move->serial_number, "path:");
        strcat(move->serial_number, path);
    }

    /* Bookkeeping of open handles (for psmove_reinit) */
    psmove_num_open_handles++;

    return move;
}

PSMove *
psmove_connect_remote_by_id(int id, moved_client *client, int remote_id)
{
    PSMove *move = (PSMove*)calloc(1, sizeof(PSMove));
    move->type = PSMove_MOVED;

    move->client = client;
    move->remote_id = remote_id;

    move->id = id;

    /* Message type for LED set requests */
    move->leds.type = PSMove_Req_SetLEDs;

    /* Remember the ID/index */
    move->id = id;

    /* Remember the serial number */
    move->serial_number = (char*)calloc(PSMOVE_MAX_SERIAL_LENGTH, sizeof(char));
    snprintf(move->serial_number, PSMOVE_MAX_SERIAL_LENGTH, "%s:%d",
            client->hostname, remote_id);

    /* Bookkeeping of open handles (for psmove_reinit) */
    psmove_num_open_handles++;

    return move;
}

PSMove *
psmove_connect_by_id(int id)
{
    int hidapi_count = psmove_count_connected_hidapi();

    if (id >= hidapi_count) {
        /* XXX: check remote controllers */
        if (clients == NULL && !psmove_remote_disabled) {
            clients = moved_client_list_open();
        }

        int offset = hidapi_count;

        moved_client_list *cur;
        for (cur=clients; cur != NULL; cur=cur->next) {
            int count = psmove_count_connected_moved(cur->client);
            if ((id - offset) < count) {
                int remote_id = id - offset;
                return psmove_connect_remote_by_id(id, cur->client, remote_id);
            }
            offset += count;
        }

        return NULL;
    }

    struct hid_device_info *devs, *cur_dev;
    int count = 0;
    PSMove *move = NULL;

    devs = hid_enumerate(PSMOVE_VID, PSMOVE_PID);
    cur_dev = devs;
    while (cur_dev) {
#ifdef _WIN32
        if (strstr(cur_dev->path, "&col02#") != NULL) {
#endif
            if (count == id) {
                move = psmove_connect_internal(cur_dev->serial_number,
                        cur_dev->path, id);
                break;
            }
#ifdef _WIN32
        } else {
            count--;
        }
#endif

        count++;
        cur_dev = cur_dev->next;
    }
    hid_free_enumeration(devs);

    return move;
}


PSMove *
psmove_connect()
{
    return psmove_connect_by_id(0);
}

int
psmove_get_btaddr(PSMove *move, PSMove_Data_BTAddr *addr)
{
    unsigned char cal[PSMOVE_CALIBRATION_SIZE];
    unsigned char btg[PSMOVE_BTADDR_GET_SIZE];
    int res;
    int i;

    psmove_return_val_if_fail(move != NULL, 0);

    if (move->type == PSMove_MOVED) {
        psmove_CRITICAL("Not implemented in MOVED mode");
        return 0;
    }

    /* Get calibration data */
    memset(cal, 0, sizeof(cal));
    cal[0] = PSMove_Req_GetCalibration;
    res = hid_get_feature_report(move->handle, cal, sizeof(cal));
    if (res < 0) {
        return 0;
    }

    /* Get Bluetooth address */
    memset(btg, 0, sizeof(btg));
    btg[0] = PSMove_Req_GetBTAddr;
    res = hid_get_feature_report(move->handle, btg, sizeof(btg));

    if (res == sizeof(btg)) {
#ifdef PSMOVE_DEBUG
        fprintf(stderr, "[PSMOVE] controller bt mac addr: ");
        for (i=6; i>=1; i--) {
            if (i != 6) putc(':', stderr);
            fprintf(stderr, "%02x", btg[i]);
        }
        fprintf(stderr, "\n");

        fprintf(stderr, "[PSMOVE] current host bt mac addr: ");
        for (i=15; i>=10; i--) {
            if (i != 15) putc(':', stderr);
            fprintf(stderr, "%02x", btg[i]);
        }
        fprintf(stderr, "\n");
#endif /* PSMOVE_DEBUG */
        if (addr != NULL) {
            /* Copy 6 bytes (btg[10]..btg[15]) into addr, reversed */
            for (i=0; i<6; i++) {
                (*addr)[i] = btg[15-i];
            }
            memcpy(addr, btg+10, 6);
        }

        /* Success! */
        return 1;
    }

    return 0;
}

const char*
psmove_get_serial(PSMove *move)
{
    psmove_return_val_if_fail(move != NULL, 0);
    psmove_return_val_if_fail(move->serial_number != NULL, 0);
    return move->serial_number;
}

int
psmove_controller_btaddr(PSMove *move, PSMove_Data_BTAddr *addr)
{
    psmove_return_val_if_fail(move != NULL, 0);
    psmove_return_val_if_fail(addr != NULL, 0);

    memcpy(addr, move->btaddr, sizeof(PSMove_Data_BTAddr));

    return 1;
}

int
psmove_set_btaddr(PSMove *move, PSMove_Data_BTAddr *addr)
{
    unsigned char bts[PSMOVE_BTADDR_SET_SIZE];
    int res;
    int i;

    psmove_return_val_if_fail(move != NULL, 0);
    psmove_return_val_if_fail(addr != NULL, 0);

    /* Get calibration data */
    memset(bts, 0, sizeof(bts));
    bts[0] = PSMove_Req_SetBTAddr;

    /* Copy 6 bytes from addr into bts[1]..bts[6], reversed */
    for (i=0; i<6; i++) {
        bts[1+5-i] = (*addr)[i];
    }

    res = hid_send_feature_report(move->handle, bts, sizeof(bts));

    return (res == sizeof(bts));
}

int
psmove_pair(PSMove *move)
{
    psmove_return_val_if_fail(move != NULL, 0);

    PSMove_Data_BTAddr btaddr;
    int i;

    if (!psmove_get_btaddr(move, &btaddr)) {
        return 0;
    }


#if defined(__APPLE__)
    BluetoothDeviceAddress address;
    IOReturn result;

    result = IOBluetoothLocalDeviceReadAddress(&address, NULL, NULL, NULL);

    if (result != kIOReturnSuccess) {
        return 0;
    }

    for (i=0; i<6; i++) {
        btaddr[i] = address.data[i];
    }

#elif defined(__linux)
    hci_for_each_dev(HCI_UP, _psmove_linux_bt_dev_info, (long)btaddr);
#elif defined(_WIN32)
    HBLUETOOTH_RADIO_FIND hFind;
    HANDLE hRadio;
    BLUETOOTH_RADIO_INFO radioInfo;

    BLUETOOTH_FIND_RADIO_PARAMS btfrp;
    btfrp.dwSize = sizeof(BLUETOOTH_FIND_RADIO_PARAMS);
    hFind = BluetoothFindFirstRadio(&btfrp, &hRadio);

    psmove_return_val_if_fail(hFind != NULL, 0);
    radioInfo.dwSize = sizeof(BLUETOOTH_RADIO_INFO);

    if (BluetoothGetRadioInfo(hRadio, &radioInfo) != ERROR_SUCCESS) {
        psmove_CRITICAL("BluetoothGetRadioInfo");
        CloseHandle(hRadio);
        BluetoothFindRadioClose(hFind);
        return 0;
    }

    for (i=0; i<6; i++) {
        btaddr[i] = radioInfo.address.rgBytes[5-i];
    }

    CloseHandle(hRadio);
    BluetoothFindRadioClose(hFind);

#else
    /* TODO: Implement for other OSes (if any?) */
    return 0;
#endif

    if (!psmove_set_btaddr(move, &btaddr)) {
        return 0;
    }

    return 1;
}

int
psmove_pair_custom(PSMove *move, const char *btaddr_string)
{
    psmove_return_val_if_fail(move != NULL, 0);

    PSMove_Data_BTAddr btaddr;

    if (!psmove_get_btaddr(move, &btaddr)) {
        return 0;
    }

    if (!psmove_btaddr_from_string(btaddr_string, &btaddr)) {
        return 0;
    }

    if (!psmove_set_btaddr(move, &btaddr)) {
        return 0;
    }

    return 1;
}

enum PSMove_Connection_Type
psmove_connection_type(PSMove *move)
{
#if defined(_WIN32)
    return move->is_bluetooth?Conn_Bluetooth:Conn_USB;
#else
    wchar_t wstr[255];
    int res;

    psmove_return_val_if_fail(move != NULL, Conn_Unknown);

    if (move->type == PSMove_MOVED) {
        return Conn_Bluetooth;
    }

    wstr[0] = 0x0000;
    res = hid_get_serial_number_string(move->handle,
            wstr, sizeof(wstr)/sizeof(wstr[0]));

    /**
     * As it turns out, we don't get a serial number when connected via USB,
     * so assume that when the serial number length is zero, then we have the
     * USB connection type, and if we have a greater-than-zero length, then it
     * is a Bluetooth connection.
     **/
    if (res == 0) {
        return Conn_USB;
    } else if (res > 0) {
        return Conn_Bluetooth;
    }

    return Conn_Unknown;
#endif
}

int
psmove_btaddr_from_string(const char *string, PSMove_Data_BTAddr *dest)
{
    int i, value;
    PSMove_Data_BTAddr tmp = {0};

    /* strlen("00:11:22:33:44:55") == 17 */
    psmove_return_val_if_fail(strlen(string) == 17, 0);

    for (i=0; i<6; i++) {
        value = strtol(string + i*3, NULL, 16);
        psmove_return_val_if_fail(value >= 0x00 && value <= 0xFF, 0);
        tmp[i] = value;
    }

    if (dest != NULL) {
        memcpy(dest, tmp, sizeof(PSMove_Data_BTAddr));
    }

    return 1;
}

void
psmove_set_leds(PSMove *move, unsigned char r, unsigned char g,
        unsigned char b)
{
    psmove_return_if_fail(move != NULL);
    move->leds.r = r;
    move->leds.g = g;
    move->leds.b = b;
}

void
psmove_set_rumble(PSMove *move, unsigned char rumble)
{
    psmove_return_if_fail(move != NULL);
    move->leds.rumble2 = 0x00;
    move->leds.rumble = rumble;
}

int
psmove_update_leds(PSMove *move)
{
    int res;

    psmove_return_val_if_fail(move != NULL, 0);

    switch (move->type) {
        case PSMove_HIDAPI:
            res = hid_write(move->handle, (unsigned char*)(&(move->leds)),
                    sizeof(move->leds));
            return (res == sizeof(move->leds));
            break;
        case PSMove_MOVED:
            moved_client_send(move->client, MOVED_REQ_WRITE,
                    move->remote_id, (unsigned char*)(&move->leds));
            return 1; // XXX
            break;
        default:
            psmove_CRITICAL("Unknown device type");
            return 0;
            break;
    }
}

int
psmove_poll(PSMove *move)
{
    int res = 0;

    psmove_return_val_if_fail(move != NULL, 0);

#ifdef PSMOVE_DEBUG
    /* store old sequence number before reading */
    int oldseq = (move->input.buttons4 & 0x0F);
#endif

    switch (move->type) {
        case PSMove_HIDAPI:
            res = hid_read(move->handle, (unsigned char*)(&(move->input)),
                sizeof(move->input));
            break;
        case PSMove_MOVED:
            if (moved_client_send(move->client, MOVED_REQ_READ,
                        move->remote_id, NULL)) {
                /**
                 * The input buffer is stored at offset 1 (the first byte
                 * contains the return value of the remote psmove_poll())
                 **/
                memcpy((unsigned char*)(&(move->input)),
                        move->client->read_response_buf+1,
                        sizeof(move->input));

                /**
                 * The first byte of the response contains the return value
                 * of the remote psmove_poll() call - if it is nonzero, we
                 * want to calculate a non-zero value locally (see below).
                 *
                 * See also _psmove_read_data() for how the buffer is filled
                 * on the remote end of the moved protocol connection.
                 **/
                if (move->client->read_response_buf[0] != 0) {
                    res = sizeof(move->input);
                }
            }
            break;
        default:
            psmove_CRITICAL("Unknown device type");
    }

    if (res == sizeof(move->input)) {
        /* Sanity check: The first byte should be PSMove_Req_GetInput */
        psmove_return_val_if_fail(move->input.type == PSMove_Req_GetInput, 0);

        /**
         * buttons4's 4 least significant bits contain the sequence number,
         * so we add 1 to signal "success" and add the sequence number for
         * consumers to utilize the data
         **/
#ifdef PSMOVE_DEBUG
        int seq = (move->input.buttons4 & 0x0F);
        if (seq != ((oldseq + 1) % 16)) {
            fprintf(stderr, "[PSMOVE] Warning: Dropped frames (seq %d -> %d)\n",
                    oldseq, seq);
        }
#endif
        return 1 + (move->input.buttons4 & 0x0F);
    }

    return 0;
}

unsigned int
psmove_get_buttons(PSMove *move)
{
    psmove_return_val_if_fail(move != NULL, 0);

    return ((move->input.buttons2) |
            (move->input.buttons1 << 8) |
            ((move->input.buttons3 & 0x01) << 16) |
            ((move->input.buttons4 & 0xF0) << 13));
}

unsigned char
psmove_get_battery(PSMove *move)
{
    psmove_return_val_if_fail(move != NULL, 0);

    return move->input.battery;
}

int
psmove_get_temperature(PSMove *move)
{
    psmove_return_val_if_fail(move != NULL, 0);

    return ((move->input.temphigh << 4) |
            ((move->input.templow_mXhigh & 0xF0) >> 4));
}

unsigned char
psmove_get_trigger(PSMove *move)
{
    psmove_return_val_if_fail(move != NULL, 0);

    return (move->input.trigger + move->input.trigger2) / 2;
}

void
psmove_get_accelerometer(PSMove *move, int *ax, int *ay, int *az)
{
    psmove_return_if_fail(move != NULL);

    if (ax != NULL) {
        *ax = ((move->input.aXlow + move->input.aXlow2) +
               ((move->input.aXhigh + move->input.aXhigh2) << 8)) / 2 - 0x8000;
    }

    if (ay != NULL) {
        *ay = ((move->input.aYlow + move->input.aYlow2) +
               ((move->input.aYhigh + move->input.aYhigh2) << 8)) / 2 - 0x8000;
    }

    if (az != NULL) {
        *az = ((move->input.aZlow + move->input.aZlow2) +
               ((move->input.aZhigh + move->input.aZhigh2) << 8)) / 2 - 0x8000;
    }
}

void
psmove_get_gyroscope(PSMove *move, int *gx, int *gy, int *gz)
{
    psmove_return_if_fail(move != NULL);

    if (gx != NULL) {
        *gx = ((move->input.gXlow + move->input.gXlow2) +
               ((move->input.gXhigh + move->input.gXhigh2) << 8)) / 2 - 0x8000;
    }

    if (gy != NULL) {
        *gy = ((move->input.gYlow + move->input.gYlow2) +
               ((move->input.gYhigh + move->input.gYhigh2) << 8)) / 2 - 0x8000;
    }

    if (gz != NULL) {
        *gz = ((move->input.gZlow + move->input.gZlow2) +
               ((move->input.gZhigh + move->input.gZhigh2) << 8)) / 2 - 0x8000;
    }
}

void
psmove_get_magnetometer(PSMove *move, int *mx, int *my, int *mz)
{
    psmove_return_if_fail(move != NULL);

    if (mx != NULL) {
        *mx = TWELVE_BIT_SIGNED(((move->input.templow_mXhigh & 0x0F) << 8) |
                move->input.mXlow);
    }

    if (my != NULL) {
        *my = TWELVE_BIT_SIGNED((move->input.mYhigh << 4) |
               (move->input.mYlow_mZhigh & 0xF0) >> 4);
    }

    if (mz != NULL) {
        *mz = TWELVE_BIT_SIGNED(((move->input.mYlow_mZhigh & 0x0F) << 8) |
                move->input.mZlow);
    }
}

void
psmove_disconnect(PSMove *move)
{
    psmove_return_if_fail(move != NULL);

    switch (move->type) {
        case PSMove_HIDAPI:
            hid_close(move->handle);
            break;
        case PSMove_MOVED:
            // XXX: Close connection?
            break;
    }

    free(move->serial_number);
    free(move);

    /* Bookkeeping of open handles (for psmove_reinit) */
    psmove_return_if_fail(psmove_num_open_handles > 0);
    psmove_num_open_handles--;
}


