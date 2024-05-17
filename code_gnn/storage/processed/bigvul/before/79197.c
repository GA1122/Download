static int StreamTcpPacketIsBadWindowUpdate(TcpSession *ssn, Packet *p)
{
    TcpStream *stream = NULL, *ostream = NULL;
    uint32_t seq;
    uint32_t ack;
    uint32_t pkt_win;

    if (p->flags & PKT_PSEUDO_STREAM_END)
        return 0;

    if (ssn->state < TCP_ESTABLISHED || ssn->state == TCP_CLOSED)
        return 0;

    if ((p->tcph->th_flags & (TH_SYN|TH_FIN|TH_RST)) != 0)
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

    if (pkt_win < ostream->window) {
        uint32_t diff = ostream->window - pkt_win;
        if (diff > p->payload_len &&
                SEQ_GT(ack, ostream->next_seq) &&
                SEQ_GT(seq, stream->next_seq))
        {
            SCLogDebug("%"PRIu64", pkt_win %u, stream win %u, diff %u, dsize %u",
                p->pcap_cnt, pkt_win, ostream->window, diff, p->payload_len);
            SCLogDebug("%"PRIu64", pkt_win %u, stream win %u",
                p->pcap_cnt, pkt_win, ostream->window);
            SCLogDebug("%"PRIu64", seq %u ack %u ostream->next_seq %u ostream->last_ack %u, ostream->next_win %u, diff %u (%u)",
                    p->pcap_cnt, seq, ack, ostream->next_seq, ostream->last_ack, ostream->next_win,
                    ostream->next_seq - ostream->last_ack, stream->next_seq - stream->last_ack);

             
            uint32_t adiff = ack - ostream->last_ack;
            if (((pkt_win > 1024) && (diff > (adiff + 32))) ||
                ((pkt_win <= 1024) && (diff > adiff)))
            {
                SCLogDebug("pkt ACK %u is %u bytes beyond last_ack %u, shrinks window by %u "
                        "(allowing 32 bytes extra): pkt WIN %u", ack, adiff, ostream->last_ack, diff, pkt_win);
                SCLogDebug("%u - %u = %u (state %u)", diff, adiff, diff - adiff, ssn->state);
                StreamTcpSetEvent(p, STREAM_PKT_BAD_WINDOW_UPDATE);
                return 1;
            }
        }

    }
    SCLogDebug("seq %u (%u), ack %u (%u)", seq, stream->next_seq, ack, ostream->last_ack);
     return 0;
 }