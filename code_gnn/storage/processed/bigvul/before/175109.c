 SetRSSIMonitorCommand(wifi_request_id id, wifi_interface_handle handle,
                s8 max_rssi, s8 min_rssi, wifi_rssi_event_handler eh)
 : WifiCommand("SetRSSIMonitorCommand", handle, id), mMax_rssi(max_rssi), mMin_rssi
 (min_rssi), mHandler(eh)
 {
 }
