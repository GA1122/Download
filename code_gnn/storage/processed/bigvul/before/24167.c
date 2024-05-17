ar6000_control_tx(void *devt, void *osbuf, HTC_ENDPOINT_ID eid)
{
    struct ar6_softc       *ar = (struct ar6_softc *)devt;
    int         status = 0;
    struct ar_cookie *cookie = NULL;
    int i;
#ifdef CONFIG_PM
    if (ar->arWowState != WLAN_WOW_STATE_NONE) {
        A_NETBUF_FREE(osbuf);
        return A_EACCES;
    }
#endif  
         
    AR6000_SPIN_LOCK(&ar->arLock, 0);

    do {

        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_TX,("ar_contrstatus = ol_tx: skb=0x%lx, len=0x%x eid =%d\n",
                         (unsigned long)osbuf, A_NETBUF_LEN(osbuf), eid));

        if (ar->arWMIControlEpFull && (eid == ar->arControlEp)) {
                 
            cookie = NULL;
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,(" WMI Control EP full, dropping packet : 0x%lX, len:%d \n",
                    (unsigned long)osbuf, A_NETBUF_LEN(osbuf)));
        } else {
            cookie = ar6000_alloc_cookie(ar);
        }

        if (cookie == NULL) {
            status = A_NO_MEMORY;
            break;
        }

        if(logWmiRawMsgs) {
            A_PRINTF("WMI cmd send, msgNo %d :", wmiSendCmdNum);
            for(i = 0; i < a_netbuf_to_len(osbuf); i++)
                A_PRINTF("%x ", ((u8 *)a_netbuf_to_data(osbuf))[i]);
            A_PRINTF("\n");
        }

        wmiSendCmdNum++;

    } while (false);

    if (cookie != NULL) {
             
        ar->arTxPending[eid]++;

        if (eid != ar->arControlEp) {
            ar->arTotalTxDataPending++;
        }
    }

    AR6000_SPIN_UNLOCK(&ar->arLock, 0);

    if (cookie != NULL) {
        cookie->arc_bp[0] = (unsigned long)osbuf;
        cookie->arc_bp[1] = 0;
        SET_HTC_PACKET_INFO_TX(&cookie->HtcPkt,
                               cookie,
                               A_NETBUF_DATA(osbuf),
                               A_NETBUF_LEN(osbuf),
                               eid,
                               AR6K_CONTROL_PKT_TAG);
             
        HTCSendPkt(ar->arHtcTarget, &cookie->HtcPkt);
        status = 0;
    }

    if (status) {
        A_NETBUF_FREE(osbuf);
    }
    return status;
}