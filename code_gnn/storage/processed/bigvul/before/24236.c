ar6000_tx_complete(void *Context, struct htc_packet_queue *pPacketQueue)
{
    struct ar6_softc     *ar = (struct ar6_softc *)Context;
    u32 mapNo = 0;
    int        status;
    struct ar_cookie * ar_cookie;
    HTC_ENDPOINT_ID   eid;
    bool          wakeEvent = false;
    struct sk_buff_head  skb_queue;
    struct htc_packet      *pPacket;
    struct sk_buff  *pktSkb;
    bool          flushing = false;

    skb_queue_head_init(&skb_queue);

         
    AR6000_SPIN_LOCK(&ar->arLock, 0);

         
    while (!HTC_QUEUE_EMPTY(pPacketQueue)) {

        pPacket = HTC_PACKET_DEQUEUE(pPacketQueue);

        ar_cookie = (struct ar_cookie *)pPacket->pPktContext;
        A_ASSERT(ar_cookie);

        status = pPacket->Status;
        pktSkb = (struct sk_buff *)ar_cookie->arc_bp[0];
        eid = pPacket->Endpoint;
        mapNo = ar_cookie->arc_bp[1];

        A_ASSERT(pktSkb);
        A_ASSERT(pPacket->pBuffer == A_NETBUF_DATA(pktSkb));

             
        __skb_queue_tail(&skb_queue,pktSkb);

        if (!status) {
            A_ASSERT(pPacket->ActualLength == A_NETBUF_LEN(pktSkb));
        }

        AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_TX,("ar6000_tx_complete skb=0x%lx data=0x%lx len=0x%x eid=%d ",
                         (unsigned long)pktSkb, (unsigned long)pPacket->pBuffer,
                         pPacket->ActualLength,
                         eid));

        ar->arTxPending[eid]--;

        if ((eid  != ar->arControlEp) || bypasswmi) {
            ar->arTotalTxDataPending--;
        }

        if (eid == ar->arControlEp)
        {
            if (ar->arWMIControlEpFull) {
                     
                ar->arWMIControlEpFull = false;
            }

            if (ar->arTxPending[eid] == 0) {
                wakeEvent = true;
            }
        }

        if (status) {
            if (status == A_ECANCELED) {
                     
                flushing = true;
            }
            AR6000_STAT_INC(ar, tx_errors);
            if (status != A_NO_RESOURCE) {
                AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("%s() -TX ERROR, status: 0x%x\n", __func__,
                            status));
            }
        } else {
            AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_TX,("OK\n"));
            flushing = false;
            AR6000_STAT_INC(ar, tx_packets);
            ar->arNetStats.tx_bytes += A_NETBUF_LEN(pktSkb);
#ifdef ADAPTIVE_POWER_THROUGHPUT_CONTROL
            aptcTR.bytesTransmitted += a_netbuf_to_len(pktSkb);
            applyAPTCHeuristics(ar);
#endif  
        }

        if ((ar->arNetworkType == ADHOC_NETWORK) && ar->arIbssPsEnable
            && (eid != ar->arControlEp) && mapNo)
        {
            mapNo --;
            ar->arNodeMap[mapNo].txPending --;

            if (!ar->arNodeMap[mapNo].txPending && (mapNo == (ar->arNodeNum - 1))) {
                u32 i;
                for (i = ar->arNodeNum; i > 0; i --) {
                    if (!ar->arNodeMap[i - 1].txPending) {
                        A_MEMZERO(&ar->arNodeMap[i - 1], sizeof(struct ar_node_mapping));
                        ar->arNodeNum --;
                    } else {
                        break;
                    }
                }
            }
        }

        ar6000_free_cookie(ar, ar_cookie);

        if (ar->arNetQueueStopped) {
            ar->arNetQueueStopped = false;
        }
    }

    AR6000_SPIN_UNLOCK(&ar->arLock, 0);

     

         
    while (!skb_queue_empty(&skb_queue)) {
             
        pktSkb = __skb_dequeue(&skb_queue);
        A_NETBUF_FREE(pktSkb);
    }

    if ((ar->arConnected == true) || bypasswmi) {
        if (!flushing) {
                 
            netif_wake_queue(ar->arNetDev);
        }
    }

    if (wakeEvent) {
        wake_up(&arEvent);
    }

}
