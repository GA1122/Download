int ar6000_target_config_wlan_params(struct ar6_softc *ar)
{
    int status = 0;

#ifdef CONFIG_HOST_TCMD_SUPPORT
    if (ar->arTargetMode != AR6000_WLAN_MODE) {
        return 0;
    }
#endif  

     
    if ((wmi_set_rx_frame_format_cmd(ar->arWmi,ar->rxMetaVersion, processDot11Hdr, processDot11Hdr)) != 0) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Unable to set the rx frame format.\n"));
        status = A_ERROR;
    }

    status = ath6kl_config_btcoex_params(ar);
    if (status)
	return status;

#if WLAN_CONFIG_IGNORE_POWER_SAVE_FAIL_EVENT_DURING_SCAN
    if ((wmi_pmparams_cmd(ar->arWmi, 0, 1, 0, 0, 1, IGNORE_POWER_SAVE_FAIL_EVENT_DURING_SCAN)) != 0) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Unable to set power save fail event policy\n"));
        status = A_ERROR;
    }
#endif

#if WLAN_CONFIG_DONOT_IGNORE_BARKER_IN_ERP
    if ((wmi_set_lpreamble_cmd(ar->arWmi, 0, WMI_DONOT_IGNORE_BARKER_IN_ERP)) != 0) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Unable to set barker preamble policy\n"));
        status = A_ERROR;
    }
#endif

    if ((wmi_set_keepalive_cmd(ar->arWmi, WLAN_CONFIG_KEEP_ALIVE_INTERVAL)) != 0) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Unable to set keep alive interval\n"));
        status = A_ERROR;
    }

#if WLAN_CONFIG_DISABLE_11N
    {
        WMI_SET_HT_CAP_CMD htCap;

        memset(&htCap, 0, sizeof(WMI_SET_HT_CAP_CMD));
        htCap.band = 0;
        if ((wmi_set_ht_cap_cmd(ar->arWmi, &htCap)) != 0) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Unable to set ht capabilities \n"));
            status = A_ERROR;
        }

        htCap.band = 1;
        if ((wmi_set_ht_cap_cmd(ar->arWmi, &htCap)) != 0) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Unable to set ht capabilities \n"));
            status = A_ERROR;
        }
    }
#endif  

#ifdef ATH6K_CONFIG_OTA_MODE
    if ((wmi_powermode_cmd(ar->arWmi, MAX_PERF_POWER)) != 0) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Unable to set power mode \n"));
        status = A_ERROR;
    }
#endif

    if ((wmi_disctimeout_cmd(ar->arWmi, WLAN_CONFIG_DISCONNECT_TIMEOUT)) != 0) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Unable to set disconnect timeout \n"));
        status = A_ERROR;
    }

#if WLAN_CONFIG_DISABLE_TX_BURSTING  
    if ((wmi_set_wmm_txop(ar->arWmi, WMI_TXOP_DISABLED)) != 0) {
        AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("Unable to set txop bursting \n"));
        status = A_ERROR;
    }
#endif 

    return status;
}