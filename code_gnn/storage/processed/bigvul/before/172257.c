static jobject android_net_wifi_get_tdls_capabilities(JNIEnv *env, jclass cls, jint iface) {

 JNIHelper helper(env);
    wifi_tdls_capabilities tdls_capabilities;
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    wifi_error ret = hal_fn.wifi_get_tdls_capabilities(handle, &tdls_capabilities);

 if (WIFI_SUCCESS == ret) {
 JNIObject<jobject> capabilities = helper.createObject(
 "com/android/server/wifi/WifiNative$TdlsCapabilities");
         helper.setIntField(capabilities, "maxConcurrentTdlsSessionNumber",
                 tdls_capabilities.max_concurrent_tdls_session_num);
         helper.setBooleanField(capabilities, "isGlobalTdlsSupported",
                 tdls_capabilities.is_global_tdls_supported == 1);
         helper.setBooleanField(capabilities, "isPerMacTdlsSupported",
                 tdls_capabilities.is_per_mac_tdls_supported == 1);
         helper.setBooleanField(capabilities, "isOffChannelTdlsSupported",
                 tdls_capabilities.is_off_channel_tdls_supported);

         ALOGD("TDLS Max Concurrent Tdls Session Number is: %d",
                 tdls_capabilities.max_concurrent_tdls_session_num);
         ALOGD("Global Tdls is: %s", tdls_capabilities.is_global_tdls_supported == 1 ? "support" :
 "not support");
         ALOGD("Per Mac Tdls is: %s", tdls_capabilities.is_per_mac_tdls_supported == 1 ? "support" :
 "not support");
         ALOGD("Off Channel Tdls is: %s", tdls_capabilities.is_off_channel_tdls_supported == 1 ?
 "support" : "not support");

 return capabilities.detach();
 } else {
 return NULL;
 }
}
