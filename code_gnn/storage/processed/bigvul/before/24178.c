ar6000_disconnect_event(struct ar6_softc *ar, u8 reason, u8 *bssid,
                        u8 assocRespLen, u8 *assocInfo, u16 protocolReasonStatus)
{
    u8 i;
    unsigned long flags;
    union iwreq_data wrqu;

    if(ar->arNetworkType & AP_NETWORK) {
        union iwreq_data wrqu;
        struct sk_buff *skb;

        if(!remove_sta(ar, bssid, protocolReasonStatus)) {
            return;
        }

         
        if (ar->sta_list_index == 0) {
            A_MUTEX_LOCK(&ar->mcastpsqLock);
            while (!A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq)) {
                skb = A_NETBUF_DEQUEUE(&ar->mcastpsq);
                A_NETBUF_FREE(skb);
            }
            A_MUTEX_UNLOCK(&ar->mcastpsqLock);

             
            if (ar->arWmiReady) {
                wmi_set_pvb_cmd(ar->arWmi, MCAST_AID, 0);
            }
        }

        if(!IS_MAC_BCAST(bssid)) {
             
            A_MEMZERO(&wrqu, sizeof(wrqu));
            memcpy(wrqu.addr.sa_data, bssid, ATH_MAC_LEN);
            wireless_send_event(ar->arNetDev, IWEVEXPIRED, &wrqu, NULL);
        }

        ar->arConnected = false;
        return;
    }

    ar6k_cfg80211_disconnect_event(ar, reason, bssid,
                                   assocRespLen, assocInfo,
                                   protocolReasonStatus);

     
    A_MEMZERO(&wrqu, sizeof(wrqu));
    wrqu.addr.sa_family = ARPHRD_ETHER;
    wireless_send_event(ar->arNetDev, SIOCGIWAP, &wrqu, NULL);

     
    aggr_reset_state(ar->aggr_cntxt);

    A_UNTIMEOUT(&ar->disconnect_timer);

    A_PRINTF("AR6000 disconnected");
    if (bssid[0] || bssid[1] || bssid[2] || bssid[3] || bssid[4] || bssid[5]) {
        A_PRINTF(" from %2.2x:%2.2x:%2.2x:%2.2x:%2.2x:%2.2x ",
                 bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
    }

    AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("\nDisconnect Reason is %d", reason));
    AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("\nProtocol Reason/Status Code is %d", protocolReasonStatus));
    AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("\nAssocResp Frame = %s",
                    assocRespLen ? " " : "NULL"));
    for (i = 0; i < assocRespLen; i++) {
        if (!(i % 0x10)) {
            AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("\n"));
        }
        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("%2.2x ", assocInfo[i]));
    }
    AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_CONNECT,("\n"));
     
    if( reason == DISCONNECT_CMD)
    {
        if ((!ar->arUserBssFilter) && (ar->arWmiReady)) {
            wmi_bssfilter_cmd(ar->arWmi, NONE_BSS_FILTER, 0);
        }
    } else {
        ar->arConnectPending = true;
        if (((reason == ASSOC_FAILED) && (protocolReasonStatus == 0x11)) ||
            ((reason == ASSOC_FAILED) && (protocolReasonStatus == 0x0) && (reconnect_flag == 1))) {
            ar->arConnected = true;
            return;
        }
    }

    if ((reason == NO_NETWORK_AVAIL) && (ar->arWmiReady)) 
    {
        bss_t *pWmiSsidnode = NULL;

         
        wmi_free_node (ar->arWmi, bssid);

         
        do
        {
             
            pWmiSsidnode = wmi_find_Ssidnode (ar->arWmi, ar->arSsid, ar->arSsidLen, false, true);

            if (pWmiSsidnode)
            {
                wmi_free_node (ar->arWmi, pWmiSsidnode->ni_macaddr);
            }

        } while (pWmiSsidnode);
    }

     
    spin_lock_irqsave(&ar->arLock, flags);
    ar->arConnected = false;
    netif_carrier_off(ar->arNetDev);
    spin_unlock_irqrestore(&ar->arLock, flags);

    if( (reason != CSERV_DISCONNECT) || (reconnect_flag != 1) ) {
        reconnect_flag = 0;
    }

    if (reason != CSERV_DISCONNECT)
    {
        ar->user_savedkeys_stat = USER_SAVEDKEYS_STAT_INIT;
        ar->user_key_ctrl      = 0;
    }

    netif_stop_queue(ar->arNetDev);
    A_MEMZERO(ar->arBssid, sizeof(ar->arBssid));
    ar->arBssChannel = 0;
    ar->arBeaconInterval = 0;

    ar6000_TxDataCleanup(ar);
}
