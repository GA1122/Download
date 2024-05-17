ar6000_rx_refill(void *Context, HTC_ENDPOINT_ID Endpoint)
{
    struct ar6_softc  *ar = (struct ar6_softc *)Context;
    void        *osBuf;
    int         RxBuffers;
    int         buffersToRefill;
    struct htc_packet  *pPacket;
    struct htc_packet_queue queue;

    buffersToRefill = (int)AR6000_MAX_RX_BUFFERS -
                                    HTCGetNumRecvBuffers(ar->arHtcTarget, Endpoint);

    if (buffersToRefill <= 0) {
             
        return;
    }

    INIT_HTC_PACKET_QUEUE(&queue);

    AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_RX,("ar6000_rx_refill: providing htc with %d buffers at eid=%d\n",
                    buffersToRefill, Endpoint));

    for (RxBuffers = 0; RxBuffers < buffersToRefill; RxBuffers++) {
        osBuf = A_NETBUF_ALLOC(AR6000_BUFFER_SIZE);
        if (NULL == osBuf) {
            break;
        }
             
        pPacket = (struct htc_packet *)(A_NETBUF_HEAD(osBuf));
             
        SET_HTC_PACKET_INFO_RX_REFILL(pPacket,osBuf,A_NETBUF_DATA(osBuf),AR6000_BUFFER_SIZE,Endpoint);
             
        HTC_PACKET_ENQUEUE(&queue,pPacket);
    }

    if (!HTC_QUEUE_EMPTY(&queue)) {
             
        HTCAddReceivePktMultiple(ar->arHtcTarget, &queue);
    }

}