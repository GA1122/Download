TmEcode StreamTcp (ThreadVars *tv, Packet *p, void *data, PacketQueue *pq, PacketQueue *postpq)
{
    StreamTcpThread *stt = (StreamTcpThread *)data;

    SCLogDebug("p->pcap_cnt %"PRIu64, p->pcap_cnt);

    if (!(PKT_IS_TCP(p))) {
        return TM_ECODE_OK;
    }

    if (p->flow == NULL) {
        StatsIncr(tv, stt->counter_tcp_no_flow);
        return TM_ECODE_OK;
    }

     

    if (!(p->flags & PKT_PSEUDO_STREAM_END)) {
        if (stream_config.flags & STREAMTCP_INIT_FLAG_CHECKSUM_VALIDATION) {
            if (StreamTcpValidateChecksum(p) == 0) {
                StatsIncr(tv, stt->counter_tcp_invalid_checksum);
                return TM_ECODE_OK;
            }
        } else {
            p->flags |= PKT_IGNORE_CHECKSUM;
        }
    } else {
        p->flags |= PKT_IGNORE_CHECKSUM;  
    }
    AppLayerProfilingReset(stt->ra_ctx->app_tctx);

    (void)StreamTcpPacket(tv, p, stt, pq);

    return TM_ECODE_OK;
}