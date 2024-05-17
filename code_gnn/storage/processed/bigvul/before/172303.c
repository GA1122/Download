static void onHotlistApLost(wifi_request_id id,
 unsigned num_results, wifi_scan_result *results) {

 JNIHelper helper(mVM);
    ALOGD("onHotlistApLost called, vm = %p, obj = %p, num_results = %d", mVM, mCls, num_results);

 JNIObject<jobjectArray> scanResults = helper.newObjectArray(num_results,
 "android/net/wifi/ScanResult", NULL);
 if (scanResults == NULL) {
        ALOGE("Error in allocating array");
 return;
 }

 for (unsigned i = 0; i < num_results; i++) {

 JNIObject<jobject> scanResult = createScanResult(helper, &results[i]);
 if (scanResult == NULL) {
            ALOGE("Error in creating scan result");
 return;
 }

        helper.setObjectArrayElement(scanResults, i, scanResult);

        ALOGD("Lost AP %32s", results[i].ssid);
 }

    helper.reportEvent(mCls, "onHotlistApLost", "(I[Landroid/net/wifi/ScanResult;)V",
        id, scanResults.get());
}
