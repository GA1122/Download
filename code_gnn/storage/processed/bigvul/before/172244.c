static jstring android_net_wifi_getInterfaceName(JNIEnv *env, jclass cls, jint i) {

 char buf[EVENT_BUF_SIZE];

 JNIHelper helper(env);

    jlong value = helper.getStaticLongArrayField(cls, WifiIfaceHandleVarName, i);
    wifi_interface_handle handle = (wifi_interface_handle) value;
 int result = hal_fn.wifi_get_iface_name(handle, buf, sizeof(buf));
 if (result < 0) {
 return NULL;
 } else {
 JNIObject<jstring> name = helper.newStringUTF(buf);
 return name.detach();
 }
}
