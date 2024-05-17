static jboolean android_net_wifi_getScanCapabilities(
 JNIEnv *env, jclass cls, jint iface, jobject capabilities) {

 JNIHelper helper(env);
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);

    wifi_gscan_capabilities c;
    memset(&c, 0, sizeof(c));
 int result = hal_fn.wifi_get_gscan_capabilities(handle, &c);
 if (result != WIFI_SUCCESS) {
        ALOGD("failed to get capabilities : %d", result);
 return JNI_FALSE;
 }

    helper.setIntField(capabilities, "max_scan_cache_size", c.max_scan_cache_size);
    helper.setIntField(capabilities, "max_scan_buckets", c.max_scan_buckets);
    helper.setIntField(capabilities, "max_ap_cache_per_scan", c.max_ap_cache_per_scan);
    helper.setIntField(capabilities, "max_rssi_sample_size", c.max_rssi_sample_size);
    helper.setIntField(capabilities, "max_scan_reporting_threshold", c.max_scan_reporting_threshold);
    helper.setIntField(capabilities, "max_hotlist_bssids", c.max_hotlist_bssids);
    helper.setIntField(capabilities, "max_significant_wifi_change_aps",
                c.max_significant_wifi_change_aps);

 return JNI_TRUE;
}
