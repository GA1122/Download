static int StreamTcpPacketIsKeepAliveACK(TcpSession *ssn, Packet *p)
{
    TcpStream *stream = NULL, *ostream = NULL;
    uint32_t seq;
    uint32_t ack;
    uint32_t pkt_win;

    if (p->flags & PKT_PSEUDO_STREAM_END)
        return 0;
     
    if (p->payload_len > 0)
        return 0;

    if ((p->tcph->th_flags & (TH_SYN|TH_FIN|TH_RST)) != 0)
        return 0;

    if (TCP_GET_WINDOW(p) == 0)
        return 0;

    if (PKT_IS_TOSERVER(p)) {
        stream = &ssn->client;
        ostream = &ssn->server;
    } else {
        stream = &ssn->server;
        ostream = &ssn->client;
    }

    seq = TCP_GET_SEQ(p);
    ack = TCP_GET_ACK(p);

    pkt_win = TCP_GET_WINDOW(p) << ostream->wscale;
    if (pkt_win != ostream->window)
        return 0;

    if ((ostream->flags & STREAMTCP_STREAM_FLAG_KEEPALIVE) && ack == ostream->last_ack && seq == stream->next_seq) {
        SCLogDebug("packet is TCP keep-aliveACK: %"PRIu64, p->pcap_cnt);
        ostream->flags &= ~STREAMTCP_STREAM_FLAG_KEEPALIVE;
        return 1;
    }
    SCLogDebug("seq %u (%u), ack %u (%u) FLAG_KEEPALIVE: %s", seq, stream->next_seq, ack, ostream->last_ack,
            ostream->flags & STREAMTCP_STREAM_FLAG_KEEPALIVE ? "set" : "not set");
    return 0;
}