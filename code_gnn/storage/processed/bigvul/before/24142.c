ar6000_acl_data_tx(struct sk_buff *skb, struct net_device *dev)
{
    struct ar6_softc        *ar = (struct ar6_softc *)ar6k_priv(dev);
    struct ar_cookie *cookie;
    HTC_ENDPOINT_ID    eid = ENDPOINT_UNUSED;

    cookie = NULL;
    AR6000_SPIN_LOCK(&ar->arLock, 0);

         
        eid = arAc2EndpointID (ar, 0);
         
        cookie = ar6000_alloc_cookie(ar);

        if (cookie != NULL) {
             
            ar->arTxPending[eid]++;
            ar->arTotalTxDataPending++;
        }


    AR6000_SPIN_UNLOCK(&ar->arLock, 0);

        if (cookie != NULL) {
            cookie->arc_bp[0] = (unsigned long)skb;
            cookie->arc_bp[1] = 0;
            SET_HTC_PACKET_INFO_TX(&cookie->HtcPkt,
                            cookie,
                            A_NETBUF_DATA(skb),
                            A_NETBUF_LEN(skb),
                            eid,
                            AR6K_DATA_PKT_TAG);

             
            HTCSendPkt(ar->arHtcTarget, &cookie->HtcPkt);
        } else {
             
            A_NETBUF_FREE(skb);
            AR6000_STAT_INC(ar, tx_dropped);
            AR6000_STAT_INC(ar, tx_aborted_errors);
        }
    return 0;
}
