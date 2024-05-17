static int HandleEstablishedPacketToClient(ThreadVars *tv, TcpSession *ssn, Packet *p,
                        StreamTcpThread *stt, PacketQueue *pq)
{
    SCLogDebug("ssn %p: =+ pkt (%" PRIu32 ") is to client: SEQ %" PRIu32 ","
               " ACK %" PRIu32 ", WIN %"PRIu16"", ssn, p->payload_len,
                TCP_GET_SEQ(p), TCP_GET_ACK(p), TCP_GET_WINDOW(p));

    if (StreamTcpValidateAck(ssn, &ssn->client, p) == -1) {
        SCLogDebug("ssn %p: rejecting because of invalid ack value", ssn);
        StreamTcpSetEvent(p, STREAM_EST_INVALID_ACK);
        return -1;
    }

     
    if ((ssn->flags & STREAMTCP_FLAG_MIDSTREAM) &&
            (ssn->flags & STREAMTCP_FLAG_MIDSTREAM_ESTABLISHED))
    {
        ssn->server.window = TCP_GET_WINDOW(p) << ssn->server.wscale;
        ssn->server.next_win = ssn->server.last_ack + ssn->server.window;
        ssn->flags &= ~STREAMTCP_FLAG_MIDSTREAM_ESTABLISHED;
        SCLogDebug("ssn %p: adjusted midstream ssn->server.next_win to "
                "%" PRIu32 "", ssn, ssn->server.next_win);
    }

     
    if ((p->payload_len == 0 || p->payload_len == 1) &&
            (TCP_GET_SEQ(p) == (ssn->server.next_seq - 1))) {
        SCLogDebug("ssn %p: pkt is keep alive", ssn);

     
    } else if (!(SEQ_GEQ((TCP_GET_SEQ(p)+p->payload_len), ssn->server.last_ack))) {
        if (ssn->flags & STREAMTCP_FLAG_ASYNC) {

            SCLogDebug("ssn %p: client => Asynchrouns stream, packet SEQ"
                    " %" PRIu32 ", payload size %" PRIu32 " (%" PRIu32 "),"
                    " ssn->client.last_ack %" PRIu32 ", ssn->client.next_win"
                    " %"PRIu32"(%"PRIu32")", ssn, TCP_GET_SEQ(p),
                    p->payload_len, TCP_GET_SEQ(p) + p->payload_len,
                    ssn->server.last_ack, ssn->server.next_win,
                    TCP_GET_SEQ(p) + p->payload_len - ssn->server.next_win);

            ssn->server.last_ack = TCP_GET_SEQ(p);

         
        } else if (SEQ_GT(ssn->server.last_ack, ssn->server.next_seq) &&
                   SEQ_GT((TCP_GET_SEQ(p)+p->payload_len),ssn->server.next_seq))
        {
            SCLogDebug("ssn %p: PKT SEQ %"PRIu32" payload_len %"PRIu16
                    " before last_ack %"PRIu32", after next_seq %"PRIu32":"
                    " acked data that we haven't seen before",
                    ssn, TCP_GET_SEQ(p), p->payload_len, ssn->server.last_ack, ssn->server.next_seq);
            if (SEQ_EQ(TCP_GET_SEQ(p),ssn->server.next_seq)) {
                StreamTcpUpdateNextSeq(ssn, &ssn->server, (ssn->server.next_seq + p->payload_len));
            }
        } else {
            SCLogDebug("ssn %p: PKT SEQ %"PRIu32" payload_len %"PRIu16
                    " before last_ack %"PRIu32". next_seq %"PRIu32,
                    ssn, TCP_GET_SEQ(p), p->payload_len, ssn->server.last_ack, ssn->server.next_seq);
            StreamTcpSetEvent(p, STREAM_EST_PKT_BEFORE_LAST_ACK);
            return -1;
        }
    }

    int zerowindowprobe = 0;
     
    if (p->payload_len == 1 && TCP_GET_SEQ(p) == ssn->server.next_seq && ssn->server.window == 0) {
        SCLogDebug("ssn %p: zero window probe", ssn);
        zerowindowprobe = 1;

     
    } else if (SEQ_EQ(ssn->server.next_seq, TCP_GET_SEQ(p))) {
        StreamTcpUpdateNextSeq(ssn, &ssn->server, (ssn->server.next_seq + p->payload_len));

     
    } else if (SEQ_LT(TCP_GET_SEQ(p),ssn->server.next_seq) &&
               SEQ_GT((TCP_GET_SEQ(p)+p->payload_len), ssn->server.next_seq))
    {
        StreamTcpUpdateNextSeq(ssn, &ssn->server, (TCP_GET_SEQ(p) + p->payload_len));
        SCLogDebug("ssn %p: ssn->server.next_seq %" PRIu32
                   " (started before next_seq, ended after)",
                   ssn, ssn->server.next_seq);

     
    } else if (SEQ_LT(ssn->server.next_seq, ssn->server.last_ack)) {
        StreamTcpUpdateNextSeq(ssn, &ssn->server, (TCP_GET_SEQ(p) + p->payload_len));
        SCLogDebug("ssn %p: ssn->server.next_seq %"PRIu32
                   " (next_seq had fallen behind last_ack)",
                   ssn, ssn->server.next_seq);

    } else {
        SCLogDebug("ssn %p: no update to ssn->server.next_seq %"PRIu32
                   " SEQ %u SEQ+ %u last_ack %u",
                   ssn, ssn->server.next_seq,
                   TCP_GET_SEQ(p), TCP_GET_SEQ(p)+p->payload_len, ssn->server.last_ack);
    }

    if (zerowindowprobe) {
        SCLogDebug("ssn %p: zero window probe, skipping oow check", ssn);
    } else if (SEQ_LEQ(TCP_GET_SEQ(p) + p->payload_len, ssn->server.next_win) ||
            (ssn->flags & (STREAMTCP_FLAG_MIDSTREAM|STREAMTCP_FLAG_ASYNC)))
    {
        SCLogDebug("ssn %p: seq %"PRIu32" in window, ssn->server.next_win "
                "%" PRIu32 "", ssn, TCP_GET_SEQ(p), ssn->server.next_win);
        ssn->client.window = TCP_GET_WINDOW(p) << ssn->client.wscale;
        SCLogDebug("ssn %p: ssn->client.window %"PRIu32"", ssn,
                    ssn->client.window);

        StreamTcpUpdateLastAck(ssn, &ssn->client, TCP_GET_ACK(p));

        if (ssn->flags & STREAMTCP_FLAG_TIMESTAMP) {
            StreamTcpHandleTimestamp(ssn, p);
        }

        StreamTcpSackUpdatePacket(&ssn->client, p);

        StreamTcpUpdateNextWin(ssn, &ssn->client, (ssn->client.last_ack + ssn->client.window));

        StreamTcpReassembleHandleSegment(tv, stt->ra_ctx, ssn, &ssn->server, p, pq);
    } else {
        SCLogDebug("ssn %p: client => SEQ out of window, packet SEQ"
                   "%" PRIu32 ", payload size %" PRIu32 " (%" PRIu32 "),"
                   " ssn->server.last_ack %" PRIu32 ", ssn->server.next_win "
                   "%" PRIu32 "(%"PRIu32")", ssn, TCP_GET_SEQ(p),
                   p->payload_len, TCP_GET_SEQ(p) + p->payload_len,
                   ssn->server.last_ack, ssn->server.next_win,
                   TCP_GET_SEQ(p) + p->payload_len - ssn->server.next_win);
        StreamTcpSetEvent(p, STREAM_EST_PACKET_OUT_OF_WINDOW);
        return -1;
    }
    return 0;
}