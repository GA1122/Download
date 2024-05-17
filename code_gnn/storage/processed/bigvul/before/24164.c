ar6000_connect_event(struct ar6_softc *ar, u16 channel, u8 *bssid,
                     u16 listenInterval, u16 beaconInterval,
                     NETWORK_TYPE networkType, u8 beaconIeLen,
                     u8 assocReqLen, u8 assocRespLen,
                     u8 *assocInfo)
{
    union iwreq_data wrqu;
    int i, beacon_ie_pos, assoc_resp_ie_pos, assoc_req_ie_pos;
    static const char *tag1 = "ASSOCINFO(ReqIEs=";
    static const char *tag2 = "ASSOCRESPIE=";
    static const char *beaconIetag = "BEACONIE=";
    char buf[WMI_CONTROL_MSG_MAX_LEN * 2 + strlen(tag1) + 1];
    char *pos;
    u8 key_op_ctrl;
    unsigned long flags;
    struct ieee80211req_key *ik;
    CRYPTO_TYPE keyType = NONE_CRYPT;

    if(ar->arNetworkType & AP_NETWORK) {
        struct net_device *dev = ar->arNetDev;
        if(memcmp(dev->dev_addr, bssid, ATH_MAC_LEN)==0) {
            ar->arACS = channel;
            ik = &ar->ap_mode_bkey;

            switch(ar->arAuthMode) {
            case NONE_AUTH:
                if(ar->arPairwiseCrypto == WEP_CRYPT) {
                    ar6000_install_static_wep_keys(ar);
                }
#ifdef WAPI_ENABLE
                else if(ar->arPairwiseCrypto == WAPI_CRYPT) {
                    ap_set_wapi_key(ar, ik);
                }
#endif
                break;
            case WPA_PSK_AUTH:
            case WPA2_PSK_AUTH:
            case (WPA_PSK_AUTH|WPA2_PSK_AUTH):
                switch (ik->ik_type) {
                    case IEEE80211_CIPHER_TKIP:
                        keyType = TKIP_CRYPT;
                        break;
                    case IEEE80211_CIPHER_AES_CCM:
                        keyType = AES_CRYPT;
                        break;
                    default:
                       goto skip_key;
                }
                wmi_addKey_cmd(ar->arWmi, ik->ik_keyix, keyType, GROUP_USAGE,
                                ik->ik_keylen, (u8 *)&ik->ik_keyrsc,
                                ik->ik_keydata, KEY_OP_INIT_VAL, ik->ik_macaddr,
                                SYNC_BOTH_WMIFLAG);

                break;
            }
skip_key:
            ar->arConnected  = true;
            return;
        }

        A_PRINTF("NEW STA %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x \n "
            " AID=%d \n", bssid[0], bssid[1], bssid[2],
             bssid[3], bssid[4], bssid[5], channel);
        switch ((listenInterval>>8)&0xFF) {
            case OPEN_AUTH:
                A_PRINTF("AUTH: OPEN\n");
                break;
            case SHARED_AUTH:
                A_PRINTF("AUTH: SHARED\n");
                break;
            default:
                A_PRINTF("AUTH: Unknown\n");
                break;
        }
        switch (listenInterval&0xFF) {
            case WPA_PSK_AUTH:
                A_PRINTF("KeyMgmt: WPA-PSK\n");
                break;
            case WPA2_PSK_AUTH:
                A_PRINTF("KeyMgmt: WPA2-PSK\n");
                break;
            default:
                A_PRINTF("KeyMgmt: NONE\n");
                break;
        }
        switch (beaconInterval) {
            case AES_CRYPT:
                A_PRINTF("Cipher: AES\n");
                break;
            case TKIP_CRYPT:
                A_PRINTF("Cipher: TKIP\n");
                break;
            case WEP_CRYPT:
                A_PRINTF("Cipher: WEP\n");
                break;
#ifdef WAPI_ENABLE
            case WAPI_CRYPT:
                A_PRINTF("Cipher: WAPI\n");
                break;
#endif
            default:
                A_PRINTF("Cipher: NONE\n");
                break;
        }

        add_new_sta(ar, bssid, channel  ,
            assocInfo  , assocRespLen  ,
            listenInterval&0xFF  , beaconInterval  ,
            (listenInterval>>8)&0xFF  );

         
        A_MEMZERO(&wrqu, sizeof(wrqu));
        memcpy(wrqu.addr.sa_data, bssid, ATH_MAC_LEN);
        wireless_send_event(ar->arNetDev, IWEVREGISTERED, &wrqu, NULL);
         
        netif_wake_queue(ar->arNetDev);
        return;
    }

    ar6k_cfg80211_connect_event(ar, channel, bssid,
                                listenInterval, beaconInterval,
                                networkType, beaconIeLen,
                                assocReqLen, assocRespLen,
                                assocInfo);

    memcpy(ar->arBssid, bssid, sizeof(ar->arBssid));
    ar->arBssChannel = channel;

    A_PRINTF("AR6000 connected event on freq %d ", channel);
    A_PRINTF("with bssid %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x "
            " listenInterval=%d, beaconInterval = %d, beaconIeLen = %d assocReqLen=%d"
            " assocRespLen =%d\n",
             bssid[0], bssid[1], bssid[2],
             bssid[3], bssid[4], bssid[5],
             listenInterval, beaconInterval,
             beaconIeLen, assocReqLen, assocRespLen);
    if (networkType & ADHOC_NETWORK) {
        if (networkType & ADHOC_CREATOR) {
            A_PRINTF("Network: Adhoc (Creator)\n");
        } else {
            A_PRINTF("Network: Adhoc (Joiner)\n");
        }
    } else {
        A_PRINTF("Network: Infrastructure\n");
    }

    if ((ar->arNetworkType == INFRA_NETWORK)) {
        wmi_listeninterval_cmd(ar->arWmi, ar->arListenIntervalT, ar->arListenIntervalB);
    }

    if (beaconIeLen && (sizeof(buf) > (9 + beaconIeLen * 2))) {
        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("\nBeaconIEs= "));

        beacon_ie_pos = 0;
        A_MEMZERO(buf, sizeof(buf));
        sprintf(buf, "%s", beaconIetag);
        pos = buf + 9;
        for (i = beacon_ie_pos; i < beacon_ie_pos + beaconIeLen; i++) {
            AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("%2.2x ", assocInfo[i]));
            sprintf(pos, "%2.2x", assocInfo[i]);
            pos += 2;
        }
        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("\n"));

        A_MEMZERO(&wrqu, sizeof(wrqu));
        wrqu.data.length = strlen(buf);
        wireless_send_event(ar->arNetDev, IWEVCUSTOM, &wrqu, buf);
    }

    if (assocRespLen && (sizeof(buf) > (12 + (assocRespLen * 2))))
    {
        assoc_resp_ie_pos = beaconIeLen + assocReqLen +
                            sizeof(u16)  +   
                            sizeof(u16)  +   
                            sizeof(u16)  ;   
        A_MEMZERO(buf, sizeof(buf));
        sprintf(buf, "%s", tag2);
        pos = buf + 12;
        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("\nAssocRespIEs= "));
         
        for (i = assoc_resp_ie_pos; i < assoc_resp_ie_pos + assocRespLen - 6; i++)
        {
            AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("%2.2x ", assocInfo[i]));
            sprintf(pos, "%2.2x", assocInfo[i]);
            pos += 2;
        }
        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("\n"));

        A_MEMZERO(&wrqu, sizeof(wrqu));
        wrqu.data.length = strlen(buf);
        wireless_send_event(ar->arNetDev, IWEVCUSTOM, &wrqu, buf);
    }

    if (assocReqLen && (sizeof(buf) > (17 + (assocReqLen * 2)))) {
         
        assoc_req_ie_pos =  beaconIeLen +
                            sizeof(u16)  +   
                            sizeof(u16);     

        A_MEMZERO(buf, sizeof(buf));
        sprintf(buf, "%s", tag1);
        pos = buf + 17;
        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("AssocReqIEs= "));
        for (i = assoc_req_ie_pos; i < assoc_req_ie_pos + assocReqLen - 4; i++) {
            AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("%2.2x ", assocInfo[i]));
            sprintf(pos, "%2.2x", assocInfo[i]);
            pos += 2;
        }
        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("\n"));

        A_MEMZERO(&wrqu, sizeof(wrqu));
        wrqu.data.length = strlen(buf);
        wireless_send_event(ar->arNetDev, IWEVCUSTOM, &wrqu, buf);
    }

    if (ar->user_savedkeys_stat == USER_SAVEDKEYS_STAT_RUN &&
        ar->user_saved_keys.keyOk == true)
    {
        key_op_ctrl = KEY_OP_VALID_MASK & ~KEY_OP_INIT_TSC;

        if (ar->user_key_ctrl & AR6000_USER_SETKEYS_RSC_UNCHANGED) {
            key_op_ctrl &= ~KEY_OP_INIT_RSC;
        } else {
            key_op_ctrl |= KEY_OP_INIT_RSC;
        }
        ar6000_reinstall_keys(ar, key_op_ctrl);
    }

    netif_wake_queue(ar->arNetDev);

     
    spin_lock_irqsave(&ar->arLock, flags);
    ar->arConnected  = true;
    ar->arConnectPending = false;
    netif_carrier_on(ar->arNetDev);
    spin_unlock_irqrestore(&ar->arLock, flags);
     
    aggr_reset_state(ar->aggr_cntxt);
    reconnect_flag = 0;

    A_MEMZERO(&wrqu, sizeof(wrqu));
    memcpy(wrqu.addr.sa_data, bssid, IEEE80211_ADDR_LEN);
    wrqu.addr.sa_family = ARPHRD_ETHER;
    wireless_send_event(ar->arNetDev, SIOCGIWAP, &wrqu, NULL);
    if ((ar->arNetworkType == ADHOC_NETWORK) && ar->arIbssPsEnable) {
        A_MEMZERO(ar->arNodeMap, sizeof(ar->arNodeMap));
        ar->arNodeNum = 0;
        ar->arNexEpId = ENDPOINT_2;
    }
   if (!ar->arUserBssFilter) {
        wmi_bssfilter_cmd(ar->arWmi, NONE_BSS_FILTER, 0);
   }

}