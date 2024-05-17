static jboolean android_net_wifi_set_Country_Code_Hal(JNIEnv *env,jclass cls, jint iface,
        jstring country_code) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

 ScopedUtfChars chars(env, country_code);
 const char *country = chars.c_str();

    ALOGD("set country code: %s", country);
    wifi_error res = hal_fn.wifi_set_country_code(handle, country);
 return res == WIFI_SUCCESS;
}
