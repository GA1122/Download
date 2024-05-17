static jboolean android_net_wifi_reset_log_handler(JNIEnv *env, jclass cls, jint iface, jint id) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

    ALOGD("android_net_wifi_reset_alert_handler = %p", handle);
 int result = hal_fn.wifi_reset_alert_handler(id, handle);
 if (result != WIFI_SUCCESS) {
        ALOGE(" Fail to reset alert handler");
 return false;
 }

    ALOGD("android_net_wifi_reset_log_handler = %p", handle);
    result = hal_fn.wifi_reset_log_handler(id, handle);
 if (result != WIFI_SUCCESS) {
        ALOGE("Fail to reset logging handler");
 return false;
 }

 return true;
}
