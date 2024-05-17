static jboolean android_net_wifi_set_log_handler(JNIEnv *env, jclass cls, jint iface, jint id) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("android_net_wifi_set_log_handler = %p", handle);

    wifi_ring_buffer_data_handler handler;
    handler.on_ring_buffer_data = &on_ring_buffer_data;
 int result = hal_fn.wifi_set_log_handler(id, handle, handler);
 if (result != WIFI_SUCCESS) {
        ALOGE("Fail to set logging handler");
 return false;
 }

    wifi_alert_handler alert_handler;
    alert_handler.on_alert = &on_alert_data;
    result = hal_fn.wifi_set_alert_handler(id, handle, alert_handler);
 if (result != WIFI_SUCCESS) {
        ALOGE(" Fail to set alert handler");
 return false;
 }

 return true;
}
