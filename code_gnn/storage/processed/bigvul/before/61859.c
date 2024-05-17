static int rm_read_header(AVFormatContext *s)
{
    RMDemuxContext *rm = s->priv_data;
    AVStream *st;
    AVIOContext *pb = s->pb;
    unsigned int tag;
    int tag_size;
    unsigned int start_time, duration;
    unsigned int data_off = 0, indx_off = 0;
    char buf[128], mime[128];
    int flags = 0;
    int ret = -1;
    unsigned size, v;
    int64_t codec_pos;

    tag = avio_rl32(pb);
    if (tag == MKTAG('.', 'r', 'a', 0xfd)) {
         
        return rm_read_header_old(s);
    } else if (tag != MKTAG('.', 'R', 'M', 'F')) {
        return AVERROR(EIO);
    }

    tag_size = avio_rb32(pb);
    avio_skip(pb, tag_size - 8);

    for(;;) {
        if (avio_feof(pb))
            goto fail;
        tag = avio_rl32(pb);
        tag_size = avio_rb32(pb);
        avio_rb16(pb);
        av_log(s, AV_LOG_TRACE, "tag=%s size=%d\n",
               av_fourcc2str(tag), tag_size);
        if (tag_size < 10 && tag != MKTAG('D', 'A', 'T', 'A'))
            goto fail;
        switch(tag) {
        case MKTAG('P', 'R', 'O', 'P'):
             
            avio_rb32(pb);  
            avio_rb32(pb);  
            avio_rb32(pb);  
            avio_rb32(pb);  
            avio_rb32(pb);  
            duration = avio_rb32(pb);  
            s->duration = av_rescale(duration, AV_TIME_BASE, 1000);
            avio_rb32(pb);  
            indx_off = avio_rb32(pb);  
            data_off = avio_rb32(pb);  
            avio_rb16(pb);  
            flags = avio_rb16(pb);  
            break;
        case MKTAG('C', 'O', 'N', 'T'):
            rm_read_metadata(s, pb, 1);
            break;
        case MKTAG('M', 'D', 'P', 'R'):
            st = avformat_new_stream(s, NULL);
            if (!st) {
                ret = AVERROR(ENOMEM);
                goto fail;
            }
            st->id = avio_rb16(pb);
            avio_rb32(pb);  
            st->codecpar->bit_rate = avio_rb32(pb);  
            avio_rb32(pb);  
            avio_rb32(pb);  
            start_time = avio_rb32(pb);  
            avio_rb32(pb);  
            duration = avio_rb32(pb);  
            st->start_time = start_time;
            st->duration = duration;
            if(duration>0)
                s->duration = AV_NOPTS_VALUE;
            get_str8(pb, buf, sizeof(buf));  
            get_str8(pb, mime, sizeof(mime));  
            st->codecpar->codec_type = AVMEDIA_TYPE_DATA;
            st->priv_data = ff_rm_alloc_rmstream();
            if (!st->priv_data)
                return AVERROR(ENOMEM);

            size = avio_rb32(pb);
            codec_pos = avio_tell(pb);

            ffio_ensure_seekback(pb, 4);
            v = avio_rb32(pb);
            if (v == MKBETAG('M', 'L', 'T', 'I')) {
                ret = rm_read_multi(s, s->pb, st, mime);
                if (ret < 0)
                    goto fail;
                avio_seek(pb, codec_pos + size, SEEK_SET);
            } else {
                avio_skip(pb, -4);
                if (ff_rm_read_mdpr_codecdata(s, s->pb, st, st->priv_data,
                                              size, mime) < 0)
                    goto fail;
            }

            break;
        case MKTAG('D', 'A', 'T', 'A'):
            goto header_end;
        default:
             
            avio_skip(pb, tag_size - 10);
            break;
        }
    }
 header_end:
    rm->nb_packets = avio_rb32(pb);  
    if (!rm->nb_packets && (flags & 4))
        rm->nb_packets = 3600 * 25;
    avio_rb32(pb);  

    if (!data_off)
        data_off = avio_tell(pb) - 18;
    if (indx_off && (pb->seekable & AVIO_SEEKABLE_NORMAL) &&
        !(s->flags & AVFMT_FLAG_IGNIDX) &&
        avio_seek(pb, indx_off, SEEK_SET) >= 0) {
        rm_read_index(s);
        avio_seek(pb, data_off + 18, SEEK_SET);
    }

    return 0;

fail:
    rm_read_close(s);
    return ret;
}
