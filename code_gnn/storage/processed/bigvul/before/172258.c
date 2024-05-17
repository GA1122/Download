static jobject android_net_wifi_get_tdls_status(JNIEnv *env,jclass cls, jint iface,jstring addr) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

    mac_addr address;
    parseMacAddress(env, addr, address);

    wifi_tdls_status status;

    wifi_error ret;
    ret = hal_fn.wifi_get_tdls_status(handle, address, &status );

 if (ret != WIFI_SUCCESS) {
 return NULL;
 } else {
 JNIObject<jobject> tdls_status = helper.createObject(
 "com/android/server/wifi/WifiNative$TdlsStatus");
        helper.setIntField(tdls_status, "channel", status.channel);
        helper.setIntField(tdls_status, "global_operating_class", status.global_operating_class);
        helper.setIntField(tdls_status, "state", status.state);
        helper.setIntField(tdls_status, "reason", status.reason);
 return tdls_status.detach();
 }
}
