static void onPnoNetworkFound(wifi_request_id id,
 unsigned num_results, wifi_scan_result *results) {

 JNIHelper helper(mVM);

    ALOGD("onPnoNetworkFound called, vm = %p, obj = %p, num_results %u", mVM, mCls, num_results);

 if (results == 0 || num_results == 0) {
       ALOGE("onPnoNetworkFound: Error no results");
 return;
 }

    jbyte *bytes;
 JNIObject<jobjectArray> scanResults(helper, NULL);

 for (unsigned i=0; i<num_results; i++) {

 JNIObject<jobject> scanResult = createScanResult(helper, &results[i]);
 if (i == 0) {
            scanResults = helper.newObjectArray(
                    num_results, "android/net/wifi/ScanResult", scanResult);
 if (scanResults == 0) {
                ALOGD("cant allocate array");
 } else {
                ALOGD("allocated array %u", helper.getArrayLength(scanResults));
 }
 } else {
            helper.setObjectArrayElement(scanResults, i, scanResult);
 }

        ALOGD("Scan result with ie length %d, i %u, <%s> rssi=%d %02x:%02x:%02x:%02x:%02x:%02x",
                results->ie_length, i, results[i].ssid, results[i].rssi, results[i].bssid[0],
                results[i].bssid[1],results[i].bssid[2], results[i].bssid[3], results[i].bssid[4],
                results[i].bssid[5]);

  



 }


    ALOGD("calling report");

    helper.reportEvent(mCls, "onPnoNetworkFound", "(I[Landroid/net/wifi/ScanResult;)V", id,
               scanResults.get());
        ALOGD("free ref");
}
