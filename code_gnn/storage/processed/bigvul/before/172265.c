static jboolean android_net_wifi_resetHotlist(JNIEnv *env, jclass cls, jint iface, jint id) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("resetting hotlist on interface[%d] = %p", iface, handle);

 return hal_fn.wifi_reset_bssid_hotlist(id, handle) == WIFI_SUCCESS;
}
