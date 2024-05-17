static jboolean android_net_wifi_setSsidWhitelist(
 JNIEnv *env, jclass cls, jint iface, jint id, jobject list) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("configure SSID white list request [%d] = %p", id, handle);
    wifi_ssid *ssids = NULL;
 int num_ssids = 0;
 if (list != NULL) {
 size_t len = helper.getArrayLength((jobjectArray)list);
 if (len > 0) {
            ssids = (wifi_ssid *)malloc(len * sizeof (wifi_ssid));
 if (!ssids) return false;
            memset(ssids, 0, len * sizeof (wifi_ssid));
 for (unsigned int i = 0; i < len; i++) {

 JNIObject<jobject> jssid = helper.getObjectArrayElement(list, i);
 if (jssid == NULL) {
                    ALOGD("configure SSID whitelist: could not get element %d", i);
                    free(ssids);
 return false;
 }

 ScopedUtfChars chars(env, (jstring)jssid.get());
 const char *utf = chars.c_str();
 if (utf == NULL) {
                    ALOGE("Error getting sssid");
                    free(ssids);
 return false;
 }

 int slen = strnlen(utf, 33);
 if (slen <= 0 || slen > 32) {
                    ALOGE("Error wrong ssid length %d", slen);
                    free(ssids);
 return false;
 }

                memcpy(ssids[i].ssid, utf, slen);
                num_ssids++;
                ALOGD("SSID white list: added ssid %s", utf);
 }
 }
 }

    ALOGD("android_net_wifi_setSsidWhitelist Added %d sssids", num_ssids);
 return hal_fn.wifi_set_ssid_white_list(id, handle, num_ssids, ssids) == WIFI_SUCCESS;
}
