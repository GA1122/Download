static void onFullScanResult(wifi_request_id id, wifi_scan_result *result) {

 JNIHelper helper(mVM);


 JNIObject<jobject> scanResult = createScanResult(helper, result);


 JNIObject<jbyteArray> elements = helper.newByteArray(result->ie_length);
 if (elements == NULL) {
        ALOGE("Error in allocating array");
 return;
 }


    jbyte *bytes = (jbyte *)&(result->ie_data[0]);
    helper.setByteArrayRegion(elements, 0, result->ie_length, bytes);


    helper.reportEvent(mCls, "onFullScanResult", "(ILandroid/net/wifi/ScanResult;[B)V", id,
            scanResult.get(), elements.get());
}
