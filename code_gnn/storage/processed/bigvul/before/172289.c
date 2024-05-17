static jboolean android_net_wifi_untrackSignificantWifiChange(
 JNIEnv *env, jclass cls, jint iface, jint id) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("resetting significant wifi change on interface[%d] = %p", iface, handle);

 return hal_fn.wifi_reset_significant_change_handler(id, handle) == WIFI_SUCCESS;
}
