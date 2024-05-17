static JNIObject<jobject> createScanResult(JNIHelper &helper, wifi_scan_result *result) {


 JNIObject<jobject> scanResult = helper.createObject("android/net/wifi/ScanResult");
 if (scanResult == NULL) {
        ALOGE("Error in creating scan result");
 return JNIObject<jobject>(helper, NULL);
 }

    ALOGV("setting SSID to %s", result->ssid);

 if (!setSSIDField(helper, scanResult, result->ssid)) {
        ALOGE("Error on set SSID");
 return JNIObject<jobject>(helper, NULL);
 }

 char bssid[32];
    sprintf(bssid, "%02x:%02x:%02x:%02x:%02x:%02x", result->bssid[0], result->bssid[1],
        result->bssid[2], result->bssid[3], result->bssid[4], result->bssid[5]);

    helper.setStringField(scanResult, "BSSID", bssid);

    helper.setIntField(scanResult, "level", result->rssi);
    helper.setIntField(scanResult, "frequency", result->channel);
    helper.setLongField(scanResult, "timestamp", result->ts);

 return scanResult;
}
