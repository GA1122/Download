static jobject android_net_wifi_get_rtt_capabilities(JNIEnv *env, jclass cls, jint iface) {

 JNIHelper helper(env);
    wifi_rtt_capabilities rtt_capabilities;
    wifi_interface_handle handle = getIfaceHandle(helper, cls, iface);
    wifi_error ret = hal_fn.wifi_get_rtt_capabilities(handle, &rtt_capabilities);

 if(WIFI_SUCCESS == ret) {
 JNIObject<jobject> capabilities = helper.createObject(
 "android/net/wifi/RttManager$RttCapabilities");
         helper.setBooleanField(capabilities, "oneSidedRttSupported",
                 rtt_capabilities.rtt_one_sided_supported == 1);
         helper.setBooleanField(capabilities, "twoSided11McRttSupported",
                 rtt_capabilities.rtt_ftm_supported == 1);
         helper.setBooleanField(capabilities, "lciSupported",
                 rtt_capabilities.lci_support);
         helper.setBooleanField(capabilities, "lcrSupported",
                 rtt_capabilities.lcr_support);
         helper.setIntField(capabilities, "preambleSupported",
                 rtt_capabilities.preamble_support);
         helper.setIntField(capabilities, "bwSupported",
                 rtt_capabilities.bw_support);
         ALOGD("One side RTT is: %s", rtt_capabilities.rtt_one_sided_supported ==1 ? "support" :
 "not support");
         ALOGD("Two side RTT is: %s", rtt_capabilities.rtt_ftm_supported == 1 ? "support" :
 "not support");
         ALOGD("LCR is: %s", rtt_capabilities.lcr_support == 1 ? "support" : "not support");

         ALOGD("LCI is: %s", rtt_capabilities.lci_support == 1 ? "support" : "not support");

         ALOGD("Support Preamble is : %d support BW is %d", rtt_capabilities.preamble_support,
                 rtt_capabilities.bw_support);
 return capabilities.detach();
 } else {
 return NULL;
 }
}
