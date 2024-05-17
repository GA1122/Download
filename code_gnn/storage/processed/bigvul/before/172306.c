static void onRssiThresholdbreached(wifi_request_id id, u8 *cur_bssid, s8 cur_rssi) {

    ALOGD("RSSI threshold breached, cur RSSI - %d!!\n", cur_rssi);
    ALOGD("BSSID %02x:%02x:%02x:%02x:%02x:%02x\n",
            cur_bssid[0], cur_bssid[1], cur_bssid[2],
            cur_bssid[3], cur_bssid[4], cur_bssid[5]);
 JNIHelper helper(mVM);
    helper.reportEvent(mCls, "onRssiThresholdBreached", "(IB)V", id, cur_rssi);
}
