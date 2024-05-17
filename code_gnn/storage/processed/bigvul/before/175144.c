static wifi_error wifi_stop_rssi_monitoring(wifi_request_id id, wifi_interface_handle iface)
{
    ALOGD("Stopping RSSI monitor");

 if(id == -1) {
        wifi_rssi_event_handler handler;
        s8 max_rssi = 0, min_rssi = 0;
        wifi_handle handle = getWifiHandle(iface);
        memset(&handler, 0, sizeof(handler));
 SetRSSIMonitorCommand *cmd = new SetRSSIMonitorCommand(id, iface,
                                                    max_rssi, min_rssi, handler);
        cmd->cancel();
        cmd->releaseRef();
 return WIFI_SUCCESS;
 }
 return wifi_cancel_cmd(id, iface);
}
