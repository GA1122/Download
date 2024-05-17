static jboolean android_net_wifi_cancelRange(
 JNIEnv *env, jclass cls, jint iface, jint id, jobject params) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("cancelling rtt request [%d] = %p", id, handle);

    mac_addr addrs[MaxRttConfigs];
    memset(&addrs, 0, sizeof(addrs));

 int len = helper.getArrayLength((jobjectArray)params);
 if (len > MaxRttConfigs) {
 return false;
 }

 for (int i = 0; i < len; i++) {

 JNIObject<jobject> param = helper.getObjectArrayElement(params, i);
 if (param == NULL) {
            ALOGD("could not get element %d", i);
 continue;
 }

        parseMacAddress(env, param, addrs[i]);
 }

 return hal_fn.wifi_rtt_range_cancel(id, handle, len, addrs) == WIFI_SUCCESS;
}
