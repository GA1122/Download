 virtual int handleEvent(WifiEvent& event) {
        ALOGI("Got a RSSI monitor event");

        nlattr *vendor_data = event.get_attribute(NL80211_ATTR_VENDOR_DATA);
 int len = event.get_vendor_data_len();

 if (vendor_data == NULL || len == 0) {
            ALOGI("RSSI monitor: No data");
 return NL_SKIP;
 }
  
 #define RSSI_MONITOR_EVT_VERSION   1
 typedef struct {
            u8 version;
            s8 cur_rssi;
            mac_addr BSSID;
 } rssi_monitor_evt;

        rssi_monitor_evt *data = (rssi_monitor_evt *)event.get_vendor_data();

 if (data->version != RSSI_MONITOR_EVT_VERSION) {
            ALOGI("Event version mismatch %d, expected %d", data->version, RSSI_MONITOR_EVT_VERSION);
 return NL_SKIP;
 }

 if (*mHandler.on_rssi_threshold_breached) {
 (*mHandler.on_rssi_threshold_breached)(id(), data->BSSID, data->cur_rssi);
 } else {
            ALOGW("No RSSI monitor handler registered");
 }

 return NL_SKIP;
 }
