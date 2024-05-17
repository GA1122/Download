ar6000_connect_to_ap(struct ar6_softc *ar)
{
     
    if((ar->arWmiReady == true) && (ar->arSsidLen > 0) && ar->arNetworkType!=AP_NETWORK)
    {
        int status;
        if((ADHOC_NETWORK != ar->arNetworkType) &&
           (NONE_AUTH==ar->arAuthMode)          &&
           (WEP_CRYPT==ar->arPairwiseCrypto)) {
                ar6000_install_static_wep_keys(ar);
        }

        if (!ar->arUserBssFilter) {
            if (wmi_bssfilter_cmd(ar->arWmi, ALL_BSS_FILTER, 0) != 0) {
                return -EIO;
            }
        }
#ifdef WAPI_ENABLE
        if (ar->arWapiEnable)  {
            ar->arPairwiseCrypto = WAPI_CRYPT;
            ar->arPairwiseCryptoLen = 0;
            ar->arGroupCrypto = WAPI_CRYPT;
            ar->arGroupCryptoLen = 0;
            ar->arAuthMode = NONE_AUTH;
            ar->arConnectCtrlFlags |= CONNECT_IGNORE_WPAx_GROUP_CIPHER;
        }
#endif
        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("Connect called with authmode %d dot11 auth %d"\
                        " PW crypto %d PW crypto Len %d GRP crypto %d"\
                        " GRP crypto Len %d\n",
                        ar->arAuthMode, ar->arDot11AuthMode,
                        ar->arPairwiseCrypto, ar->arPairwiseCryptoLen,
                        ar->arGroupCrypto, ar->arGroupCryptoLen));
        reconnect_flag = 0;
         
        if ((ar->arNetworkType == INFRA_NETWORK)) {
            wmi_listeninterval_cmd(ar->arWmi, max(ar->arListenIntervalT, (u16)A_MAX_WOW_LISTEN_INTERVAL), 0);
        }
        status = wmi_connect_cmd(ar->arWmi, ar->arNetworkType,
                                 ar->arDot11AuthMode, ar->arAuthMode,
                                 ar->arPairwiseCrypto, ar->arPairwiseCryptoLen,
                                 ar->arGroupCrypto,ar->arGroupCryptoLen,
                                 ar->arSsidLen, ar->arSsid,
                                 ar->arReqBssid, ar->arChannelHint,
                                 ar->arConnectCtrlFlags);
        if (status) {
            wmi_listeninterval_cmd(ar->arWmi, ar->arListenIntervalT, ar->arListenIntervalB);
            if (!ar->arUserBssFilter) {
                wmi_bssfilter_cmd(ar->arWmi, NONE_BSS_FILTER, 0);
            }
            return status;
        }

        if ((!(ar->arConnectCtrlFlags & CONNECT_DO_WPA_OFFLOAD)) &&
            ((WPA_PSK_AUTH == ar->arAuthMode) || (WPA2_PSK_AUTH == ar->arAuthMode)))
        {
            A_TIMEOUT_MS(&ar->disconnect_timer, A_DISCONNECT_TIMER_INTERVAL, 0);
        }

        ar->arConnectCtrlFlags &= ~CONNECT_DO_WPA_OFFLOAD;
        
        ar->arConnectPending = true;
        return status;    
    }
    return A_ERROR;
}