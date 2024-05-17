static inline bool stellaris_txpacket_complete(stellaris_enet_state *s)
{
    int framelen = stellaris_txpacket_datalen(s);
    framelen += 16;
    if (!(s->tctl & SE_TCTL_CRC)) {
        framelen += 4;
    }
     
    framelen = MIN(framelen, ARRAY_SIZE(s->tx_fifo));
    return s->tx_fifo_len >= framelen;
}
