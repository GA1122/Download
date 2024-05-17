static HTC_SEND_FULL_ACTION ar6000_tx_queue_full(void *Context, struct htc_packet *pPacket)
{
    struct ar6_softc     *ar = (struct ar6_softc *)Context;
    HTC_SEND_FULL_ACTION    action = HTC_SEND_FULL_KEEP;
    bool                  stopNet = false;
    HTC_ENDPOINT_ID         Endpoint = HTC_GET_ENDPOINT_FROM_PKT(pPacket);

    do {

        if (bypasswmi) {
            int accessClass;

            if (HTC_GET_TAG_FROM_PKT(pPacket) == AR6K_CONTROL_PKT_TAG) {
                     
                break;
            }

            accessClass = arEndpoint2Ac(ar,Endpoint);
                 
            if ((accessClass == WMM_AC_BE) || (accessClass == WMM_AC_BK)) {
                action = HTC_SEND_FULL_DROP;
                stopNet = false;
            } else {
                     
                stopNet = true;
            }
            break;
        }

        if (Endpoint == ar->arControlEp) {
                 
            AR6000_SPIN_LOCK(&ar->arLock, 0);
                 
            ar->arWMIControlEpFull = true;
            AR6000_SPIN_UNLOCK(&ar->arLock, 0);
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("WMI Control Endpoint is FULL!!! \n"));
                 
            stopNet = false;
            break;
        }

         

        if (HTC_GET_TAG_FROM_PKT(pPacket) == AR6K_CONTROL_PKT_TAG) {
             
            break;
        }

        if (ar->arNetworkType == ADHOC_NETWORK) {
             
            stopNet = true;
            break;
        }
         
        if (ar->arAcStreamPriMap[arEndpoint2Ac(ar,Endpoint)] < ar->arHiAcStreamActivePri &&
            ar->arCookieCount <= MAX_HI_COOKIE_NUM) {
                 
            action = HTC_SEND_FULL_DROP;
                 
            stopNet = false;
            break;
        }

    } while (false);

    if (stopNet) {
        AR6000_SPIN_LOCK(&ar->arLock, 0);
        ar->arNetQueueStopped = true;
        AR6000_SPIN_UNLOCK(&ar->arLock, 0);
         
        netif_stop_queue(ar->arNetDev);
    }

    return action;
}
