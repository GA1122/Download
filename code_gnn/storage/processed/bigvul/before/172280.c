static jboolean android_net_wifi_start_logging_ring_buffer(JNIEnv *env, jclass cls, jint iface,
        jint verbose_level,jint flags, jint max_interval,jint min_data_size, jstring ring_name) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

    ALOGD("android_net_wifi_start_logging_ring_buffer = %p", handle);

 if (handle == 0) {
 return false;
 }

 ScopedUtfChars chars(env, ring_name);
 const char* ring_name_const_char = chars.c_str();
 int ret = hal_fn.wifi_start_logging(handle, verbose_level,
            flags, max_interval, min_data_size, const_cast<char *>(ring_name_const_char));

 if (ret != WIFI_SUCCESS) {
        ALOGE("Fail to start logging for ring %s", ring_name_const_char);
 } else {
        ALOGD("start logging for ring %s", ring_name_const_char);
 }

 return ret == WIFI_SUCCESS;
}
