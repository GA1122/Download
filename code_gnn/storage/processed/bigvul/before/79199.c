static int StreamTcpPacketIsKeepAlive(TcpSession *ssn, Packet *p)
{
    TcpStream *stream = NULL, *ostream = NULL;
    uint32_t seq;
    uint32_t ack;

    if (p->flags & PKT_PSEUDO_STREAM_END)
        return 0;

     
    if (p->payload_len > 1)
        return 0;

    if ((p->tcph->th_flags & (TH_SYN|TH_FIN|TH_RST)) != 0) {
        return 0;
    }

    if (PKT_IS_TOSERVER(p)) {
        stream = &ssn->client;
        ostream = &ssn->server;
    } else {
        stream = &ssn->server;
        ostream = &ssn->client;
    }

    seq = TCP_GET_SEQ(p);
    ack = TCP_GET_ACK(p);

    if (ack == ostream->last_ack && seq == (stream->next_seq - 1)) {
        SCLogDebug("packet is TCP keep-alive: %"PRIu64, p->pcap_cnt);
        stream->flags |= STREAMTCP_STREAM_FLAG_KEEPALIVE;
        return 1;
    }
    SCLogDebug("seq %u (%u), ack %u (%u)", seq,  (stream->next_seq - 1), ack, ostream->last_ack);
    return 0;
}