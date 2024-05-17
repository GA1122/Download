static wifi_error wifi_start_rssi_monitoring(wifi_request_id id, wifi_interface_handle
                        iface, s8 max_rssi, s8 min_rssi, wifi_rssi_event_handler eh)
{
    ALOGD("Start RSSI monitor %d", id);
    wifi_handle handle = getWifiHandle(iface);
 SetRSSIMonitorCommand *cmd = new SetRSSIMonitorCommand(id, iface, max_rssi, min_rssi, eh);
    wifi_register_cmd(handle, id, cmd);

    wifi_error result = (wifi_error)cmd->start();
 if (result != WIFI_SUCCESS) {
        wifi_unregister_cmd(handle, id);
 }
 return result;
}
