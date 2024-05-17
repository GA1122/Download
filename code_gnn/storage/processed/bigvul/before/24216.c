ar6000_rx(void *Context, struct htc_packet *pPacket)
{
    struct ar6_softc *ar = (struct ar6_softc *)Context;
    struct sk_buff *skb = (struct sk_buff *)pPacket->pPktContext;
    int minHdrLen;
    u8 containsDot11Hdr = 0;
    int        status = pPacket->Status;
    HTC_ENDPOINT_ID   ept = pPacket->Endpoint;

    A_ASSERT((status) ||
             (pPacket->pBuffer == (A_NETBUF_DATA(skb) + HTC_HEADER_LEN)));

    AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_RX,("ar6000_rx ar=0x%lx eid=%d, skb=0x%lx, data=0x%lx, len=0x%x status:%d",
                    (unsigned long)ar, ept, (unsigned long)skb, (unsigned long)pPacket->pBuffer,
                    pPacket->ActualLength, status));
    if (status) {
        if (status != A_ECANCELED) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("RX ERR (%d) \n",status));
        }
    }

         
    AR6000_SPIN_LOCK(&ar->arLock, 0);

    if (!status) {
        AR6000_STAT_INC(ar, rx_packets);
        ar->arNetStats.rx_bytes += pPacket->ActualLength;
#ifdef ADAPTIVE_POWER_THROUGHPUT_CONTROL
        aptcTR.bytesReceived += a_netbuf_to_len(skb);
        applyAPTCHeuristics(ar);
#endif  

        A_NETBUF_PUT(skb, pPacket->ActualLength +  HTC_HEADER_LEN);
        A_NETBUF_PULL(skb, HTC_HEADER_LEN);

#ifdef DEBUG
        if (debugdriver >= 2) {
            ar6000_dump_skb(skb);
        }
#endif  
    }

    AR6000_SPIN_UNLOCK(&ar->arLock, 0);

    skb->dev = ar->arNetDev;
    if (status) {
        AR6000_STAT_INC(ar, rx_errors);
        A_NETBUF_FREE(skb);
    } else if (ar->arWmiEnabled == true) {
        if (ept == ar->arControlEp) {
            
#ifdef CONFIG_PM 
            ar6000_check_wow_status(ar, skb, true);
#endif  
            wmi_control_rx(ar->arWmi, skb);
        } else {
                WMI_DATA_HDR *dhdr = (WMI_DATA_HDR *)A_NETBUF_DATA(skb);
                bool is_amsdu;
                u8 tid;

		 
                if (WARN_ON(WMI_DATA_HDR_GET_DATA_TYPE(dhdr) ==
			    WMI_DATA_HDR_DATA_TYPE_ACL)) {
			AR6000_STAT_INC(ar, rx_errors);
			A_NETBUF_FREE(skb);
			return;
		}

#ifdef CONFIG_PM 
                ar6000_check_wow_status(ar, NULL, false);
#endif  
                 

                if (processDot11Hdr) {
                    minHdrLen = sizeof(WMI_DATA_HDR) + sizeof(struct ieee80211_frame) + sizeof(ATH_LLC_SNAP_HDR);
                } else {
                    minHdrLen = sizeof (WMI_DATA_HDR) + sizeof(ATH_MAC_HDR) +
                          sizeof(ATH_LLC_SNAP_HDR);
                }

                 
                if (ar->arNetworkType != AP_NETWORK &&
                    ((pPacket->ActualLength < minHdrLen) ||
                    (pPacket->ActualLength > AR6000_MAX_RX_MESSAGE_SIZE)))
                {
                     
                    AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("TOO SHORT or TOO LONG\n"));
                    AR6000_STAT_INC(ar, rx_errors);
                    AR6000_STAT_INC(ar, rx_length_errors);
                    A_NETBUF_FREE(skb);
                } else {
                    u16 seq_no;
                    u8 meta_type;

#if 0
                     
                    AR_DEBUG_PRINTF(ATH_DEBUG_INFO,("RSSI %d\n",
                        ((WMI_DATA_HDR *) A_NETBUF_DATA(skb))->rssi));
#endif
                     
                    if (ar->arNetworkType == AP_NETWORK) {
                        sta_t *conn = NULL;
                        u8 psState=0,prevPsState;
                        ATH_MAC_HDR *datap=NULL;
                        u16 offset;

                        meta_type = WMI_DATA_HDR_GET_META(dhdr);

                        psState = (((WMI_DATA_HDR *)A_NETBUF_DATA(skb))->info
                                     >> WMI_DATA_HDR_PS_SHIFT) & WMI_DATA_HDR_PS_MASK;

                        offset = sizeof(WMI_DATA_HDR);

                        switch (meta_type) {
                            case 0:
                                break;
                            case WMI_META_VERSION_1:
                                offset += sizeof(WMI_RX_META_V1);
                                break;
                            case WMI_META_VERSION_2:
                                offset += sizeof(WMI_RX_META_V2);
                                break;
                            default:
                                break;
                        }

                        datap = (ATH_MAC_HDR *)(A_NETBUF_DATA(skb)+offset);
                        conn = ieee80211_find_conn(ar, datap->srcMac);

                        if (conn) {
                             
                            prevPsState = STA_IS_PWR_SLEEP(conn);
                            if (psState) {
                                STA_SET_PWR_SLEEP(conn);
                            } else {
                                STA_CLR_PWR_SLEEP(conn);
                            }

                            if (prevPsState ^ STA_IS_PWR_SLEEP(conn)) {

                                if (!STA_IS_PWR_SLEEP(conn)) {

                                    A_MUTEX_LOCK(&conn->psqLock);
                                    while (!A_NETBUF_QUEUE_EMPTY(&conn->psq)) {
                                        struct sk_buff *skb=NULL;

                                        skb = A_NETBUF_DEQUEUE(&conn->psq);
                                        A_MUTEX_UNLOCK(&conn->psqLock);
                                        ar6000_data_tx(skb,ar->arNetDev);
                                        A_MUTEX_LOCK(&conn->psqLock);
                                    }
                                    A_MUTEX_UNLOCK(&conn->psqLock);
                                     
                                    wmi_set_pvb_cmd(ar->arWmi, conn->aid, 0);
                                }
                            }
                        } else {
                             
                            A_ASSERT(false);
                        }

                         
                        if((pPacket->ActualLength < minHdrLen) ||
                                (pPacket->ActualLength > AR6000_MAX_RX_MESSAGE_SIZE)) {
                            A_NETBUF_FREE(skb);
                            goto rx_done;
                        }
                    }

                    is_amsdu = WMI_DATA_HDR_IS_AMSDU(dhdr) ? true : false;
                    tid = WMI_DATA_HDR_GET_UP(dhdr);
                    seq_no = WMI_DATA_HDR_GET_SEQNO(dhdr);
                    meta_type = WMI_DATA_HDR_GET_META(dhdr);
                    containsDot11Hdr = WMI_DATA_HDR_GET_DOT11(dhdr);

                    wmi_data_hdr_remove(ar->arWmi, skb);

                    switch (meta_type) {
                        case WMI_META_VERSION_1:
                            {
                                WMI_RX_META_V1 *pMeta = (WMI_RX_META_V1 *)A_NETBUF_DATA(skb);
                                A_PRINTF("META %d %d %d %d %x\n", pMeta->status, pMeta->rix, pMeta->rssi, pMeta->channel, pMeta->flags);
                                A_NETBUF_PULL((void*)skb, sizeof(WMI_RX_META_V1));
                                break;
                            }
                        case WMI_META_VERSION_2:
                            {
                                WMI_RX_META_V2 *pMeta = (WMI_RX_META_V2 *)A_NETBUF_DATA(skb);
                                if(pMeta->csumFlags & 0x1){
                                    skb->ip_summed=CHECKSUM_COMPLETE;
                                    skb->csum=(pMeta->csum);
                                }
                                A_NETBUF_PULL((void*)skb, sizeof(WMI_RX_META_V2));
                                break;
                            }
                        default:
                            break;
                    }

                    A_ASSERT(status == 0);

                     
                    if(containsDot11Hdr) {
                        status = wmi_dot11_hdr_remove(ar->arWmi,skb);
                    } else if(!is_amsdu) {
                        status = wmi_dot3_2_dix(skb);
                    }

                    if (status) {
                         
                        A_NETBUF_FREE(skb);
                        goto rx_done;
                    }

                    if ((ar->arNetDev->flags & IFF_UP) == IFF_UP) {
                        if (ar->arNetworkType == AP_NETWORK) {
                            struct sk_buff *skb1 = NULL;
                            ATH_MAC_HDR *datap;

                            datap = (ATH_MAC_HDR *)A_NETBUF_DATA(skb);
                            if (IEEE80211_IS_MULTICAST(datap->dstMac)) {
                                 
                                skb1 = skb_copy(skb,GFP_ATOMIC);
                            } else {
                                 
                                sta_t *conn = NULL;
                                conn = ieee80211_find_conn(ar, datap->dstMac);

                                if (conn && ar->intra_bss) {
                                    skb1 = skb;
                                    skb = NULL;
                                } else if(conn && !ar->intra_bss) {
                                    A_NETBUF_FREE(skb);
                                    skb = NULL;
                                }
                            }
                            if (skb1) {
                                ar6000_data_tx(skb1, ar->arNetDev);
                            }
                        }
                    }
                    aggr_process_recv_frm(ar->aggr_cntxt, tid, seq_no, is_amsdu, (void **)&skb);
                    ar6000_deliver_frames_to_nw_stack((void *) ar->arNetDev, (void *)skb);
                }
            }
    } else {
        if (EPPING_ALIGNMENT_PAD > 0) {
            A_NETBUF_PULL(skb, EPPING_ALIGNMENT_PAD);
        }
        ar6000_deliver_frames_to_nw_stack((void *)ar->arNetDev, (void *)skb);
    }

rx_done:

    return;
}
