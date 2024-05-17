static jboolean android_net_wifi_enable_disable_tdls(JNIEnv *env,jclass cls, jint iface,
        jboolean enable, jstring addr) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

    mac_addr address;
    parseMacAddress(env, addr, address);
    wifi_tdls_handler tdls_handler;

 if(enable) {
 return (hal_fn.wifi_enable_tdls(handle, address, NULL, tdls_handler) == WIFI_SUCCESS);
 } else {
 return (hal_fn.wifi_disable_tdls(handle, address) == WIFI_SUCCESS);
 }
}
