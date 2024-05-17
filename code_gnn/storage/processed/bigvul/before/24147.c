static struct htc_packet *ar6000_alloc_amsdu_rxbuf(void *Context, HTC_ENDPOINT_ID Endpoint, int Length)
{
    struct htc_packet  *pPacket = NULL;
    struct ar6_softc  *ar = (struct ar6_softc *)Context;
    int         refillCount = 0;

    AR_DEBUG_PRINTF(ATH_DEBUG_WLAN_RX,("ar6000_alloc_amsdu_rxbuf: eid=%d, Length:%d\n",Endpoint,Length));

    do {

        if (Length <= AR6000_BUFFER_SIZE) {
                 
            A_ASSERT(false);
            break;
        }

        if (Length > AR6000_AMSDU_BUFFER_SIZE) {
            A_ASSERT(false);
            break;
        }

        AR6000_SPIN_LOCK(&ar->arLock, 0);
             
        pPacket = HTC_PACKET_DEQUEUE(&ar->amsdu_rx_buffer_queue);
             
        refillCount = AR6000_MAX_AMSDU_RX_BUFFERS - HTC_PACKET_QUEUE_DEPTH(&ar->amsdu_rx_buffer_queue);
        AR6000_SPIN_UNLOCK(&ar->arLock, 0);

        if (NULL == pPacket) {
            break;
        }
             
        pPacket->Endpoint = Endpoint;

    } while (false);

    if (refillCount >= AR6000_AMSDU_REFILL_THRESHOLD) {
        ar6000_refill_amsdu_rxbufs(ar,refillCount);
    }

    return pPacket;
}