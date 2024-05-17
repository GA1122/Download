static int h264_handle_packet(AVFormatContext *ctx, PayloadContext *data,
                              AVStream *st, AVPacket *pkt, uint32_t *timestamp,
                              const uint8_t *buf, int len, uint16_t seq,
                              int flags)
{
    uint8_t nal;
    uint8_t type;
    int result = 0;

    if (!len) {
        av_log(ctx, AV_LOG_ERROR, "Empty H.264 RTP packet\n");
        return AVERROR_INVALIDDATA;
    }
    nal  = buf[0];
    type = nal & 0x1f;

     
    if (type >= 1 && type <= 23)
        type = 1;
    switch (type) {
    case 0:                     
    case 1:
        if ((result = av_new_packet(pkt, len + sizeof(start_sequence))) < 0)
            return result;
        memcpy(pkt->data, start_sequence, sizeof(start_sequence));
        memcpy(pkt->data + sizeof(start_sequence), buf, len);
        COUNT_NAL_TYPE(data, nal);
        break;

    case 24:                    
        buf++;
        len--;
        result = ff_h264_handle_aggregated_packet(ctx, data, pkt, buf, len, 0,
                                                  NAL_COUNTERS, NAL_MASK);
        break;

    case 25:                    
    case 26:                    
    case 27:                    
    case 29:                    
        avpriv_report_missing_feature(ctx, "RTP H.264 NAL unit type %d", type);
        result = AVERROR_PATCHWELCOME;
        break;

    case 28:                    
        result = h264_handle_packet_fu_a(ctx, data, pkt, buf, len,
                                         NAL_COUNTERS, NAL_MASK);
        break;

    case 30:                    
    case 31:                    
    default:
        av_log(ctx, AV_LOG_ERROR, "Undefined type (%d)\n", type);
        result = AVERROR_INVALIDDATA;
        break;
    }

    pkt->stream_index = st->index;

    return result;
}
