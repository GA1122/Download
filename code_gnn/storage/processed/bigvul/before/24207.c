static void ar6000_refill_amsdu_rxbufs(struct ar6_softc *ar, int Count)
{
    struct htc_packet  *pPacket;
    void        *osBuf;

    while (Count > 0) {
        osBuf = A_NETBUF_ALLOC(AR6000_AMSDU_BUFFER_SIZE);
        if (NULL == osBuf) {
            break;
        }
             
        pPacket = (struct htc_packet *)(A_NETBUF_HEAD(osBuf));
             
        SET_HTC_PACKET_INFO_RX_REFILL(pPacket,osBuf,A_NETBUF_DATA(osBuf),AR6000_AMSDU_BUFFER_SIZE,0);

        AR6000_SPIN_LOCK(&ar->arLock, 0);
             
        HTC_PACKET_ENQUEUE(&ar->amsdu_rx_buffer_queue,pPacket);
        AR6000_SPIN_UNLOCK(&ar->arLock, 0);
        Count--;
    }

}