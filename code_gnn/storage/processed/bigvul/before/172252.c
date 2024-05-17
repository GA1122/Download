static jobject android_net_wifi_get_firmware_version(JNIEnv *env, jclass cls, jint iface) {

 JNIHelper helper(env);
 int buffer_length = 256;
 char *buffer = (char *)malloc(buffer_length);
 if (!buffer) return NULL;
    memset(buffer, 0, buffer_length);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

    ALOGD("android_net_wifi_get_firmware_version = %p", handle);

 if (handle == 0) {
 return NULL;
 }

    wifi_error result = hal_fn.wifi_get_firmware_version(handle, buffer, buffer_length);

 if (result == WIFI_SUCCESS) {
        ALOGD("buffer is %p, length is %d", buffer, buffer_length);
 JNIObject<jstring> firmware_version = helper.newStringUTF(buffer);
        free(buffer);
 return firmware_version.detach();
 } else {
        ALOGD("Fail to get Firmware version");
        free(buffer);
 return NULL;
 }
}
