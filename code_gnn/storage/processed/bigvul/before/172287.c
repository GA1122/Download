static jboolean android_net_wifi_trackSignificantWifiChange(
 JNIEnv *env, jclass cls, jint iface, jint id, jobject settings) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    ALOGD("tracking significant wifi change on interface[%d] = %p", iface, handle);

    wifi_significant_change_params params;
    memset(&params, 0, sizeof(params));

    params.rssi_sample_size = helper.getIntField(settings, "rssiSampleSize");
    params.lost_ap_sample_size = helper.getIntField(settings, "lostApSampleSize");
    params.min_breaching = helper.getIntField(settings, "minApsBreachingThreshold");

 const char *bssid_info_array_type = "[Landroid/net/wifi/WifiScanner$BssidInfo;";
 JNIObject<jobjectArray> bssids = helper.getArrayField(
            settings, "bssidInfos", bssid_info_array_type);
    params.num_bssid = helper.getArrayLength(bssids);

 if (params.num_bssid == 0) {
        ALOGE("Error in accessing array");
 return false;
 }

    ALOGD("Initialized common fields %d, %d, %d, %d", params.rssi_sample_size,
            params.lost_ap_sample_size, params.min_breaching, params.num_bssid);

 for (int i = 0; i < params.num_bssid; i++) {
 JNIObject<jobject> objAp = helper.getObjectArrayElement(bssids, i);

 JNIObject<jstring> macAddrString = helper.getStringField(objAp, "bssid");
 if (macAddrString == NULL) {
            ALOGE("Error getting bssid field");
 return false;
 }

 ScopedUtfChars chars(env, macAddrString.get());
 const char *bssid = chars.c_str();
 if (bssid == NULL) {
            ALOGE("Error getting bssid");
 return false;
 }

        mac_addr addr;
        parseMacAddress(bssid, addr);
        memcpy(params.ap[i].bssid, addr, sizeof(mac_addr));

 char bssidOut[32];
        sprintf(bssidOut, "%02x:%02x:%02x:%02x:%02x:%02x", addr[0], addr[1],
            addr[2], addr[3], addr[4], addr[5]);

        params.ap[i].low = helper.getIntField(objAp, "low");
        params.ap[i].high = helper.getIntField(objAp, "high");

        ALOGD("Added bssid %s, [%04d, %04d]", bssidOut, params.ap[i].low, params.ap[i].high);
 }

    ALOGD("Added %d bssids", params.num_bssid);

    wifi_significant_change_handler handler;
    memset(&handler, 0, sizeof(handler));

    handler.on_significant_change = &onSignificantWifiChange;
 return hal_fn.wifi_set_significant_change_handler(id, handle, params, handler) == WIFI_SUCCESS;
}
