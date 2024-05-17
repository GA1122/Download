 int createRequest(WifiRequest& request, int enable) {
 int result = request.create(GOOGLE_OUI, WIFI_SUBCMD_SET_RSSI_MONITOR);
 if (result < 0) {
 return result;
 }

        nlattr *data = request.attr_start(NL80211_ATTR_VENDOR_DATA);
        result = request.put_u32(RSSI_MONITOR_ATTRIBUTE_MAX_RSSI, (enable ? mMax_rssi: 0));
 if (result < 0) {
 return result;
 }
        ALOGD("create request");
        result = request.put_u32(RSSI_MONITOR_ATTRIBUTE_MIN_RSSI, (enable? mMin_rssi: 0));
 if (result < 0) {
 return result;
 }
        result = request.put_u32(RSSI_MONITOR_ATTRIBUTE_START, enable);
 if (result < 0) {
 return result;
 }
        request.attr_end(data);
 return result;
 }
