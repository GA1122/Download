ar6000_rssiThreshold_event(struct ar6_softc *ar,  WMI_RSSI_THRESHOLD_VAL newThreshold, s16 rssi)
{
    USER_RSSI_THOLD userRssiThold;

    rssi = rssi + SIGNAL_QUALITY_NOISE_FLOOR;

    if (enablerssicompensation) {
        rssi = rssi_compensation_calc(ar, rssi);
    }

     
    userRssiThold.tag = ar->rssi_map[newThreshold].tag;
    userRssiThold.rssi = rssi;
    A_PRINTF("rssi Threshold range = %d tag = %d  rssi = %d\n", newThreshold,
             userRssiThold.tag, userRssiThold.rssi);
}