static jboolean android_net_wifi_setScanningMacOui(JNIEnv *env, jclass cls,
        jint iface, jbyteArray param) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("setting scan oui %p", handle);

 static const unsigned oui_len = 3;  
 int len = helper.getArrayLength(param);
 if (len != oui_len) {
        ALOGE("invalid oui length %d", len);
 return false;
 }

 ScopedBytesRO paramBytes(env, param);
 const jbyte* bytes = paramBytes.get();
 if (bytes == NULL) {
        ALOGE("failed to get array");
 return false;
 }

 return hal_fn.wifi_set_scanning_mac_oui(handle, (byte *)bytes) == WIFI_SUCCESS;
}
