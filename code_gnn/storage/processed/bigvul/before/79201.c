static int StreamTcpPacketIsRetransmission(TcpStream *stream, Packet *p)
{
    if (p->payload_len == 0)
        SCReturnInt(0);

     
    if (SEQ_LT(TCP_GET_SEQ(p), stream->last_ack) && SEQ_GT((TCP_GET_SEQ(p) + p->payload_len), stream->last_ack))
    {
        StreamTcpSetEvent(p, STREAM_PKT_RETRANSMISSION);
        SCReturnInt(1);
    }

     
    if (SEQ_LEQ((TCP_GET_SEQ(p) + p->payload_len), stream->last_ack)) {
        StreamTcpSetEvent(p, STREAM_PKT_RETRANSMISSION);
        SCReturnInt(1);
    }

     
    if (SEQ_LEQ((TCP_GET_SEQ(p) + p->payload_len), stream->next_seq)) {
        StreamTcpSetEvent(p, STREAM_PKT_RETRANSMISSION);
        SCReturnInt(2);
    }

    SCLogDebug("seq %u payload_len %u => %u, last_ack %u, next_seq %u", TCP_GET_SEQ(p),
            p->payload_len, (TCP_GET_SEQ(p) + p->payload_len), stream->last_ack, stream->next_seq);
    SCReturnInt(0);
}