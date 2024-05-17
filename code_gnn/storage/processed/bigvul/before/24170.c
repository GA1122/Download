ar6000_data_tx(struct sk_buff *skb, struct net_device *dev)
{
#define AC_NOT_MAPPED   99
    struct ar6_softc        *ar = (struct ar6_softc *)ar6k_priv(dev);
    u8 ac = AC_NOT_MAPPED;
    HTC_ENDPOINT_ID    eid = ENDPOINT_UNUSED;
    u32 mapNo = 0;
    int               len;
    struct ar_cookie *cookie;
    bool            checkAdHocPsMapping = false,bMoreData = false;
    HTC_TX_TAG        htc_tag = AR6K_DATA_PKT_TAG;
    u8 dot11Hdr = processDot11Hdr;
#ifdef CONFIG_PM
    if (ar->arWowState != WLAN_WOW_STATE_NONE) {
        A_NETBUF_FREE(skb);
        return 0;
    }
#endif  

    AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_TX,("ar6000_data_tx start - skb=0x%lx, data=0x%lx, len=0x%x\n",
                     (unsigned long)skb, (unsigned long)A_NETBUF_DATA(skb),
                     A_NETBUF_LEN(skb)));

     
    if( (!ar->arConnected && !bypasswmi)
#ifdef CONFIG_HOST_TCMD_SUPPORT
      
    || (ar->arTargetMode == AR6000_TCMD_MODE)
#endif
                                            ) {
        A_NETBUF_FREE(skb);
        return 0;
    }

    do {

        if (ar->arWmiReady == false && bypasswmi == 0) {
            break;
        }

#ifdef BLOCK_TX_PATH_FLAG
        if (blocktx) {
            break;
        }
#endif  

         
         

        if (ar->arNetworkType == AP_NETWORK) {
            ATH_MAC_HDR *datap = (ATH_MAC_HDR *)A_NETBUF_DATA(skb);
            sta_t *conn = NULL;

             
            if (IEEE80211_IS_MULTICAST(datap->dstMac)) {
                u8 ctr=0;
                bool qMcast=false;


                for (ctr=0; ctr<AP_MAX_NUM_STA; ctr++) {
                    if (STA_IS_PWR_SLEEP((&ar->sta_list[ctr]))) {
                        qMcast = true;
                    }
                }
                if(qMcast) {

                     
                    if (ar->DTIMExpired == false) {
                        bool isMcastqEmpty = false;

                        A_MUTEX_LOCK(&ar->mcastpsqLock);
                        isMcastqEmpty = A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq);
                        A_NETBUF_ENQUEUE(&ar->mcastpsq, skb);
                        A_MUTEX_UNLOCK(&ar->mcastpsqLock);

                         
                        if (isMcastqEmpty) {
                             wmi_set_pvb_cmd(ar->arWmi, MCAST_AID, 1);
                        }
                        return 0;
                    } else {
                      
                         A_MUTEX_LOCK(&ar->mcastpsqLock);
                         if(!A_NETBUF_QUEUE_EMPTY(&ar->mcastpsq)) {
                             bMoreData = true;
                         }
                         A_MUTEX_UNLOCK(&ar->mcastpsqLock);
                    }
                }
            } else {
                conn = ieee80211_find_conn(ar, datap->dstMac);
                if (conn) {
                    if (STA_IS_PWR_SLEEP(conn)) {
                         
                        if (!STA_IS_PS_POLLED(conn)) {
                            bool isPsqEmpty = false;
                             
                            A_MUTEX_LOCK(&conn->psqLock);
                            isPsqEmpty = A_NETBUF_QUEUE_EMPTY(&conn->psq);
                            A_NETBUF_ENQUEUE(&conn->psq, skb);
                            A_MUTEX_UNLOCK(&conn->psqLock);

                             
                            if (isPsqEmpty) {
                                wmi_set_pvb_cmd(ar->arWmi, conn->aid, 1);
                            }

                            return 0;
                         } else {
                          
                             A_MUTEX_LOCK(&conn->psqLock);
                             if (!A_NETBUF_QUEUE_EMPTY(&conn->psq)) {
                                 bMoreData = true;
                             }
                             A_MUTEX_UNLOCK(&conn->psqLock);
                         }
                    }
                } else {

                     
                    A_NETBUF_FREE(skb);
                    return 0;
                }
            }
        }

        if (ar->arWmiEnabled) {
        u8 csumStart=0;
        u8 csumDest=0;
        u8 csum=skb->ip_summed;
        if(csumOffload && (csum==CHECKSUM_PARTIAL)){
            csumStart = (skb->head + skb->csum_start - skb_network_header(skb) +
			 sizeof(ATH_LLC_SNAP_HDR));
            csumDest=skb->csum_offset+csumStart;
        }
            if (A_NETBUF_HEADROOM(skb) < dev->hard_header_len - LINUX_HACK_FUDGE_FACTOR) {
                struct sk_buff  *newbuf;

                 
                len = A_NETBUF_LEN(skb);
                newbuf = A_NETBUF_ALLOC(len);
                if (newbuf == NULL) {
                    break;
                }
                A_NETBUF_PUT(newbuf, len);
                memcpy(A_NETBUF_DATA(newbuf), A_NETBUF_DATA(skb), len);
                A_NETBUF_FREE(skb);
                skb = newbuf;
                 
            }

            if (dot11Hdr) {
                if (wmi_dot11_hdr_add(ar->arWmi,skb,ar->arNetworkType) != 0) {
                    AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("ar6000_data_tx-wmi_dot11_hdr_add failed\n"));
                    break;
                }
            } else {
                if (wmi_dix_2_dot3(ar->arWmi, skb) != 0) {
                    AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("ar6000_data_tx - wmi_dix_2_dot3 failed\n"));
                    break;
                }
            }
            if(csumOffload && (csum ==CHECKSUM_PARTIAL)){
                WMI_TX_META_V2  metaV2;
                metaV2.csumStart =csumStart;
                metaV2.csumDest = csumDest;
                metaV2.csumFlags = 0x1; 
                if (wmi_data_hdr_add(ar->arWmi, skb, DATA_MSGTYPE, bMoreData, dot11Hdr,
                                        WMI_META_VERSION_2,&metaV2) != 0) {
                    AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("ar6000_data_tx - wmi_data_hdr_add failed\n"));
                    break;
                }

            }
            else
            {
                if (wmi_data_hdr_add(ar->arWmi, skb, DATA_MSGTYPE, bMoreData, dot11Hdr,0,NULL) != 0) {
                    AR_DEBUG_PRINTF(ATH_DEBUG_ERR,("ar6000_data_tx - wmi_data_hdr_add failed\n"));
                    break;
                }
            }


            if ((ar->arNetworkType == ADHOC_NETWORK) &&
                ar->arIbssPsEnable && ar->arConnected) {
                     
                checkAdHocPsMapping = true;

            } else {
                     
                ac  =  wmi_implicit_create_pstream(ar->arWmi, skb, 0, ar->arWmmEnabled);
            }

        } else {
            EPPING_HEADER    *eppingHdr;

            eppingHdr = A_NETBUF_DATA(skb);

            if (IS_EPPING_PACKET(eppingHdr)) {
                     
                ac = eppingHdr->StreamNo_h;
                     
                if (IS_EPING_PACKET_NO_DROP(eppingHdr)) {
                    htc_tag = AR6K_CONTROL_PKT_TAG;
                }

                if (ac == HCI_TRANSPORT_STREAM_NUM) {
                         
#ifndef EXPORT_HCI_BRIDGE_INTERFACE
                    if (!hci_test_send(ar,skb)) {
                        return 0;
                    }
#endif
                         
                    ac = AC_NOT_MAPPED;
                } else {
                     
                    if (EPPING_ALIGNMENT_PAD > 0) {
                        A_NETBUF_PUSH(skb, EPPING_ALIGNMENT_PAD);
                    }
                }

            } else {
                     
                ac = AC_NOT_MAPPED;
            }
        }

    } while (false);

         
    if ((ac == AC_NOT_MAPPED) && !checkAdHocPsMapping) {
             
        A_NETBUF_FREE(skb);
        AR6000_STAT_INC(ar, tx_dropped);
        AR6000_STAT_INC(ar, tx_aborted_errors);
        return 0;
    }

    cookie = NULL;

         
    AR6000_SPIN_LOCK(&ar->arLock, 0);

    do {

        if (checkAdHocPsMapping) {
            eid = ar6000_ibss_map_epid(skb, dev, &mapNo);
        }else {
            eid = arAc2EndpointID (ar, ac);
        }
             
        if (eid == 0 || eid == ENDPOINT_UNUSED ) {
            AR_DEBUG_PRINTF(ATH_DEBUG_ERR,(" eid %d is NOT mapped!\n", eid));
            break;
        }
             
        cookie = ar6000_alloc_cookie(ar);

        if (cookie != NULL) {
                 
            ar->arTxPending[eid]++;
            ar->arTotalTxDataPending++;
        }

    } while (false);

    AR6000_SPIN_UNLOCK(&ar->arLock, 0);

    if (cookie != NULL) {
        cookie->arc_bp[0] = (unsigned long)skb;
        cookie->arc_bp[1] = mapNo;
        SET_HTC_PACKET_INFO_TX(&cookie->HtcPkt,
                               cookie,
                               A_NETBUF_DATA(skb),
                               A_NETBUF_LEN(skb),
                               eid,
                               htc_tag);

#ifdef DEBUG
        if (debugdriver >= 3) {
            ar6000_dump_skb(skb);
        }
#endif
#ifdef HTC_TEST_SEND_PKTS
        DoHTCSendPktsTest(ar,mapNo,eid,skb);
#endif
             
        HTCSendPkt(ar->arHtcTarget, &cookie->HtcPkt);
    } else {
             
        A_NETBUF_FREE(skb);
        AR6000_STAT_INC(ar, tx_dropped);
        AR6000_STAT_INC(ar, tx_aborted_errors);
    }

    return 0;
}