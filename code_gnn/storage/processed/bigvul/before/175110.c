 virtual int cancel() {

 WifiRequest request(familyId(), ifaceId());
 int result = createRequest(request, 0);
 if (result != WIFI_SUCCESS) {
            ALOGE("failed to create request; result = %d", result);
 } else {
            result = requestResponse(request);
 if (result != WIFI_SUCCESS) {
                ALOGE("failed to stop RSSI monitoring = %d", result);
 }
 }
        unregisterVendorHandler(GOOGLE_OUI, GOOGLE_RSSI_MONITOR_EVENT);
 return WIFI_SUCCESS;
 }
