static jint android_net_wifi_stop_sending_offloaded_packet(JNIEnv *env, jclass cls,
                    jint iface, jint idx) {
 int ret;
 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("Stop packet offload [%d] = %p", idx, handle);
    ret =  hal_fn.wifi_stop_sending_offloaded_packet(idx, handle);
    ALOGD("ret= %d\n", ret);
 return ret;
}
