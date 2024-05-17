static void ar6000_cleanup_amsdu_rxbufs(struct ar6_softc *ar)
{
    struct htc_packet  *pPacket;
    void        *osBuf;

         
    while (true) {

        AR6000_SPIN_LOCK(&ar->arLock, 0);
        pPacket = HTC_PACKET_DEQUEUE(&ar->amsdu_rx_buffer_queue);
        AR6000_SPIN_UNLOCK(&ar->arLock, 0);

        if (NULL == pPacket) {
            break;
        }

        osBuf = pPacket->pPktContext;
        if (NULL == osBuf) {
            A_ASSERT(false);
            break;
        }

        A_NETBUF_FREE(osBuf);
    }

}