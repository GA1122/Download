static int asf_read_replicated_data(AVFormatContext *s, ASFPacket *asf_pkt)
{
    ASFContext *asf = s->priv_data;
    AVIOContext *pb = s->pb;
    int ret, data_size;

    if (!asf_pkt->data_size) {
        data_size = avio_rl32(pb);  
        if (data_size <= 0)
            return AVERROR_INVALIDDATA;
        if ((ret = av_new_packet(&asf_pkt->avpkt, data_size)) < 0)
            return ret;
        asf_pkt->data_size = asf_pkt->size_left = data_size;
    } else
        avio_skip(pb, 4);  
    asf_pkt->dts = avio_rl32(pb);  
    if (asf->rep_data_len && (asf->rep_data_len >= 8))
        avio_skip(pb, asf->rep_data_len - 8);  

    return 0;
}
