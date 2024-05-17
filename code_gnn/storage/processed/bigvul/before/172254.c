static jboolean android_net_wifi_get_ring_buffer_data(JNIEnv *env, jclass cls, jint iface,
        jstring ring_name) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

 ScopedUtfChars chars(env, ring_name);
 const char* ring_name_const_char = chars.c_str();
 int result = hal_fn.wifi_get_ring_data(handle, const_cast<char *>(ring_name_const_char));
 return result == WIFI_SUCCESS;
}
