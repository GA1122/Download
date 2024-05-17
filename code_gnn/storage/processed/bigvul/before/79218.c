int StreamTcpSegmentForEach(const Packet *p, uint8_t flag, StreamSegmentCallback CallbackFunc, void *data)
{
    TcpSession *ssn = NULL;
    TcpStream *stream = NULL;
    int ret = 0;
    int cnt = 0;

    if (p->flow == NULL)
        return 0;

    ssn = (TcpSession *)p->flow->protoctx;

    if (ssn == NULL) {
        return 0;
    }

    if (flag & FLOW_PKT_TOSERVER) {
        stream = &(ssn->server);
    } else {
        stream = &(ssn->client);
    }

     
    TcpSegment *seg = stream->seg_list;
    for (; seg != NULL &&
            ((stream_config.flags & STREAMTCP_INIT_FLAG_INLINE)
             || SEQ_LT(seg->seq, stream->last_ack));)
    {
        const uint8_t *seg_data;
        uint32_t seg_datalen;
        StreamingBufferSegmentGetData(&stream->sb, &seg->sbseg, &seg_data, &seg_datalen);

        ret = CallbackFunc(p, data, seg_data, seg_datalen);
        if (ret != 1) {
            SCLogDebug("Callback function has failed");
            return -1;
        }
        seg = seg->next;
        cnt++;
    }
    return cnt;
}