void onLinkStatsResults(wifi_request_id id, wifi_iface_stat *iface_stat,
 int num_radios, wifi_radio_stat *radio_stats)
{
 if (iface_stat != 0) {
        memcpy(&link_stat, iface_stat, sizeof(wifi_iface_stat));
 } else {
        memset(&link_stat, 0, sizeof(wifi_iface_stat));
 }

 if (num_radios > 0 && radio_stats != 0) {
        memcpy(&radio_stat, radio_stats, sizeof(wifi_radio_stat));
 } else {
        memset(&radio_stat, 0, sizeof(wifi_radio_stat));
 }
}
