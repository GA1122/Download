static jboolean android_net_wifi_setDfsFlag(JNIEnv *env, jclass cls, jint iface, jboolean dfs) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("setting dfs flag to %s, %p", dfs ? "true" : "false", handle);

    u32 nodfs = dfs ? 0 : 1;
    wifi_error result = hal_fn.wifi_set_nodfs_flag(handle, nodfs);
 return result == WIFI_SUCCESS;
}
