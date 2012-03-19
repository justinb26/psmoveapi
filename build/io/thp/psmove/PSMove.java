/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package io.thp.psmove;

public class PSMove {
  private long swigCPtr;
  protected boolean swigCMemOwn;

  public PSMove(long cPtr, boolean cMemoryOwn) {
    swigCMemOwn = cMemoryOwn;
    swigCPtr = cPtr;
  }

  public static long getCPtr(PSMove obj) {
    return (obj == null) ? 0 : obj.swigCPtr;
  }

  protected void finalize() {
    delete();
  }

  public synchronized void delete() {
    if (swigCPtr != 0) {
      if (swigCMemOwn) {
        swigCMemOwn = false;
        psmoveapiJNI.delete_PSMove(swigCPtr);
      }
      swigCPtr = 0;
    }
  }

  public int getConnection_type() {
    return psmoveapiJNI.PSMove_connection_type_get(swigCPtr);
  }

  public int getAx() {
    return psmoveapiJNI.PSMove_ax_get(swigCPtr);
  }

  public int getAy() {
    return psmoveapiJNI.PSMove_ay_get(swigCPtr);
  }

  public int getAz() {
    return psmoveapiJNI.PSMove_az_get(swigCPtr);
  }

  public int getGx() {
    return psmoveapiJNI.PSMove_gx_get(swigCPtr);
  }

  public int getGy() {
    return psmoveapiJNI.PSMove_gy_get(swigCPtr);
  }

  public int getGz() {
    return psmoveapiJNI.PSMove_gz_get(swigCPtr);
  }

  public int getMx() {
    return psmoveapiJNI.PSMove_mx_get(swigCPtr);
  }

  public int getMy() {
    return psmoveapiJNI.PSMove_my_get(swigCPtr);
  }

  public int getMz() {
    return psmoveapiJNI.PSMove_mz_get(swigCPtr);
  }

  public PSMove() {
    this(psmoveapiJNI.new_PSMove__SWIG_0(), true);
  }

  public PSMove(int id) {
    this(psmoveapiJNI.new_PSMove__SWIG_1(id), true);
  }

  public void set_leds(int r, int g, int b) {
    psmoveapiJNI.PSMove_set_leds(swigCPtr, r, g, b);
  }

  public void set_rumble(int rumble) {
    psmoveapiJNI.PSMove_set_rumble(swigCPtr, rumble);
  }

  public int update_leds() {
    return psmoveapiJNI.PSMove_update_leds(swigCPtr);
  }

  public int pair() {
    return psmoveapiJNI.PSMove_pair(swigCPtr);
  }

  public int pair_custom(String btaddr) {
    return psmoveapiJNI.PSMove_pair_custom(swigCPtr, btaddr);
  }

  public String get_serial() {
    return psmoveapiJNI.PSMove_get_serial(swigCPtr);
  }

  public int is_remote() {
    return psmoveapiJNI.PSMove_is_remote(swigCPtr);
  }

  public int poll() {
    return psmoveapiJNI.PSMove_poll(swigCPtr);
  }

  public int get_buttons() {
    return psmoveapiJNI.PSMove_get_buttons(swigCPtr);
  }

  public int get_battery() {
    return psmoveapiJNI.PSMove_get_battery(swigCPtr);
  }

  public int get_temperature() {
    return psmoveapiJNI.PSMove_get_temperature(swigCPtr);
  }

  public int get_trigger() {
    return psmoveapiJNI.PSMove_get_trigger(swigCPtr);
  }

}