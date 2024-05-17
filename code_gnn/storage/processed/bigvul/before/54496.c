int ff_mov_read_stsd_entries(MOVContext *c, AVIOContext *pb, int entries)
{
    AVStream *st;
    MOVStreamContext *sc;
    int j, pseudo_stream_id;

    if (c->fc->nb_streams < 1)
        return 0;
    st = c->fc->streams[c->fc->nb_streams-1];
    sc = st->priv_data;

    for (pseudo_stream_id=0; pseudo_stream_id<entries; pseudo_stream_id++) {
        enum CodecID id;
        int dref_id = 1;
        MOVAtom a = { AV_RL32("stsd") };
        int64_t start_pos = avio_tell(pb);
        int size = avio_rb32(pb);  
        uint32_t format = avio_rl32(pb);  

        if (size >= 16) {
            avio_rb32(pb);  
            avio_rb16(pb);  
            dref_id = avio_rb16(pb);
        }else if (size <= 0){
            av_log(c->fc, AV_LOG_ERROR, "invalid size %d in stsd\n", size);
            return -1;
        }

        if (st->codec->codec_tag &&
            st->codec->codec_tag != format &&
            (c->fc->video_codec_id ? ff_codec_get_id(ff_codec_movvideo_tags, format) != c->fc->video_codec_id
                                   : st->codec->codec_tag != MKTAG('j','p','e','g'))
           ){
             
            av_log(c->fc, AV_LOG_WARNING, "multiple fourcc not supported\n");
            avio_skip(pb, size - (avio_tell(pb) - start_pos));
            continue;
        }
         
        if (st->codec->codec_tag && st->codec->codec_tag == AV_RL32("avc1"))
            av_log(c->fc, AV_LOG_WARNING, "Concatenated H.264 might not play corrently.\n");
        sc->pseudo_stream_id = st->codec->codec_tag ? -1 : pseudo_stream_id;
        sc->dref_id= dref_id;

        st->codec->codec_tag = format;
        id = ff_codec_get_id(ff_codec_movaudio_tags, format);
        if (id<=0 && ((format&0xFFFF) == 'm'+('s'<<8) || (format&0xFFFF) == 'T'+('S'<<8)))
            id = ff_codec_get_id(ff_codec_wav_tags, av_bswap32(format)&0xFFFF);

        if (st->codec->codec_type != AVMEDIA_TYPE_VIDEO && id > 0) {
            st->codec->codec_type = AVMEDIA_TYPE_AUDIO;
        } else if (st->codec->codec_type != AVMEDIA_TYPE_AUDIO &&  
                   format && format != MKTAG('m','p','4','s')) {  
            id = ff_codec_get_id(ff_codec_movvideo_tags, format);
            if (id <= 0)
                id = ff_codec_get_id(ff_codec_bmp_tags, format);
            if (id > 0)
                st->codec->codec_type = AVMEDIA_TYPE_VIDEO;
            else if (st->codec->codec_type == AVMEDIA_TYPE_DATA ||
                     (st->codec->codec_type == AVMEDIA_TYPE_SUBTITLE &&
                      st->codec->codec_id == CODEC_ID_NONE)){
                id = ff_codec_get_id(ff_codec_movsubtitle_tags, format);
                if (id > 0)
                    st->codec->codec_type = AVMEDIA_TYPE_SUBTITLE;
            }
        }

        av_dlog(c->fc, "size=%d 4CC= %c%c%c%c codec_type=%d\n", size,
                (format >> 0) & 0xff, (format >> 8) & 0xff, (format >> 16) & 0xff,
                (format >> 24) & 0xff, st->codec->codec_type);

        if (st->codec->codec_type==AVMEDIA_TYPE_VIDEO) {
            unsigned int color_depth, len;
            int color_greyscale;
            int color_table_id;

            st->codec->codec_id = id;
            avio_rb16(pb);  
            avio_rb16(pb);  
            avio_rb32(pb);  
            avio_rb32(pb);  
            avio_rb32(pb);  

            st->codec->width = avio_rb16(pb);  
            st->codec->height = avio_rb16(pb);  

            avio_rb32(pb);  
            avio_rb32(pb);  
            avio_rb32(pb);  
            avio_rb16(pb);  

            len = avio_r8(pb);  
            if (len > 31)
                len = 31;
            mov_read_mac_string(c, pb, len, st->codec->codec_name, 32);
            if (len < 31)
                avio_skip(pb, 31 - len);
             
            if (!memcmp(st->codec->codec_name, "Planar Y'CbCr 8-bit 4:2:0", 25))
                st->codec->codec_tag=MKTAG('I', '4', '2', '0');

            st->codec->bits_per_coded_sample = avio_rb16(pb);  
            color_table_id = avio_rb16(pb);  
            av_dlog(c->fc, "depth %d, ctab id %d\n",
                   st->codec->bits_per_coded_sample, color_table_id);
             
            color_depth = st->codec->bits_per_coded_sample & 0x1F;
            color_greyscale = st->codec->bits_per_coded_sample & 0x20;

             
            if ((color_depth == 2) || (color_depth == 4) ||
                (color_depth == 8)) {
                 
                unsigned int color_start, color_count, color_end;
                unsigned char a, r, g, b;

                if (color_greyscale) {
                    int color_index, color_dec;
                     
                    st->codec->bits_per_coded_sample = color_depth;
                    color_count = 1 << color_depth;
                    color_index = 255;
                    color_dec = 256 / (color_count - 1);
                    for (j = 0; j < color_count; j++) {
                        if (id == CODEC_ID_CINEPAK){
                            r = g = b = color_count - 1 - color_index;
                        }else
                        r = g = b = color_index;
                        sc->palette[j] =
                            (0xFFU << 24) | (r << 16) | (g << 8) | (b);
                        color_index -= color_dec;
                        if (color_index < 0)
                            color_index = 0;
                    }
                } else if (color_table_id) {
                    const uint8_t *color_table;
                     
                    color_count = 1 << color_depth;
                    if (color_depth == 2)
                        color_table = ff_qt_default_palette_4;
                    else if (color_depth == 4)
                        color_table = ff_qt_default_palette_16;
                    else
                        color_table = ff_qt_default_palette_256;

                    for (j = 0; j < color_count; j++) {
                        r = color_table[j * 3 + 0];
                        g = color_table[j * 3 + 1];
                        b = color_table[j * 3 + 2];
                        sc->palette[j] =
                            (0xFFU << 24) | (r << 16) | (g << 8) | (b);
                    }
                } else {
                     
                    color_start = avio_rb32(pb);
                    color_count = avio_rb16(pb);
                    color_end = avio_rb16(pb);
                    if ((color_start <= 255) &&
                        (color_end <= 255)) {
                        for (j = color_start; j <= color_end; j++) {
                             
                            a = avio_r8(pb);
                            avio_r8(pb);
                            r = avio_r8(pb);
                            avio_r8(pb);
                            g = avio_r8(pb);
                            avio_r8(pb);
                            b = avio_r8(pb);
                            avio_r8(pb);
                            sc->palette[j] =
                                (a << 24 ) | (r << 16) | (g << 8) | (b);
                        }
                    }
                }
                sc->has_palette = 1;
            }
        } else if (st->codec->codec_type==AVMEDIA_TYPE_AUDIO) {
            int bits_per_sample, flags;
            uint16_t version = avio_rb16(pb);

            st->codec->codec_id = id;
            avio_rb16(pb);  
            avio_rb32(pb);  

            st->codec->channels = avio_rb16(pb);              
            av_dlog(c->fc, "audio channels %d\n", st->codec->channels);
            st->codec->bits_per_coded_sample = avio_rb16(pb);       

            sc->audio_cid = avio_rb16(pb);
            avio_rb16(pb);  

            st->codec->sample_rate = ((avio_rb32(pb) >> 16));

            av_dlog(c->fc, "version =%d, isom =%d\n",version,c->isom);
            if (!c->isom) {
                if (version==1) {
                    sc->samples_per_frame = avio_rb32(pb);
                    avio_rb32(pb);  
                    sc->bytes_per_frame = avio_rb32(pb);
                    avio_rb32(pb);  
                } else if (version==2) {
                    avio_rb32(pb);  
                    st->codec->sample_rate = av_int2double(avio_rb64(pb));  
                    st->codec->channels = avio_rb32(pb);
                    avio_rb32(pb);  
                    st->codec->bits_per_coded_sample = avio_rb32(pb);  
                    flags = avio_rb32(pb);  
                    sc->bytes_per_frame = avio_rb32(pb);  
                    sc->samples_per_frame = avio_rb32(pb);  
                    if (format == MKTAG('l','p','c','m'))
                        st->codec->codec_id = ff_mov_get_lpcm_codec_id(st->codec->bits_per_coded_sample, flags);
                }
            }

            switch (st->codec->codec_id) {
            case CODEC_ID_PCM_S8:
            case CODEC_ID_PCM_U8:
                if (st->codec->bits_per_coded_sample == 16)
                    st->codec->codec_id = CODEC_ID_PCM_S16BE;
                break;
            case CODEC_ID_PCM_S16LE:
            case CODEC_ID_PCM_S16BE:
                if (st->codec->bits_per_coded_sample == 8)
                    st->codec->codec_id = CODEC_ID_PCM_S8;
                else if (st->codec->bits_per_coded_sample == 24)
                    st->codec->codec_id =
                        st->codec->codec_id == CODEC_ID_PCM_S16BE ?
                        CODEC_ID_PCM_S24BE : CODEC_ID_PCM_S24LE;
                break;
             
            case CODEC_ID_MACE3:
                sc->samples_per_frame = 6;
                sc->bytes_per_frame = 2*st->codec->channels;
                break;
            case CODEC_ID_MACE6:
                sc->samples_per_frame = 6;
                sc->bytes_per_frame = 1*st->codec->channels;
                break;
            case CODEC_ID_ADPCM_IMA_QT:
                sc->samples_per_frame = 64;
                sc->bytes_per_frame = 34*st->codec->channels;
                break;
            case CODEC_ID_GSM:
                sc->samples_per_frame = 160;
                sc->bytes_per_frame = 33;
                break;
            default:
                break;
            }

            bits_per_sample = av_get_bits_per_sample(st->codec->codec_id);
            if (bits_per_sample) {
                st->codec->bits_per_coded_sample = bits_per_sample;
                sc->sample_size = (bits_per_sample >> 3) * st->codec->channels;
            }
        } else if (st->codec->codec_type==AVMEDIA_TYPE_SUBTITLE){
            MOVAtom fake_atom = { .size = size - (avio_tell(pb) - start_pos) };
            if (format != AV_RL32("mp4s"))  
                mov_read_glbl(c, pb, fake_atom);
            st->codec->codec_id= id;
            st->codec->width = sc->width;
            st->codec->height = sc->height;
        } else {
            if (st->codec->codec_tag == MKTAG('t','m','c','d')) {
                MOVStreamContext *tmcd_ctx = st->priv_data;
                int val;
                avio_rb32(pb);        
                val = avio_rb32(pb);  
                tmcd_ctx->tmcd_flags = val;
                if (val & 1)
                    st->codec->flags2 |= CODEC_FLAG2_DROP_FRAME_TIMECODE;
                avio_rb32(pb);  
                avio_rb32(pb);  
                st->codec->time_base.den = avio_r8(pb);  
                st->codec->time_base.num = 1;
            }
             
            avio_skip(pb, size - (avio_tell(pb) - start_pos));
        }
         
        a.size = size - (avio_tell(pb) - start_pos);
        if (a.size > 8) {
            int ret;
            if ((ret = mov_read_default(c, pb, a)) < 0)
                return ret;
        } else if (a.size > 0)
            avio_skip(pb, a.size);
    }

    if (st->codec->codec_type==AVMEDIA_TYPE_AUDIO && st->codec->sample_rate==0 && sc->time_scale>1)
        st->codec->sample_rate= sc->time_scale;

     
    switch (st->codec->codec_id) {
#if CONFIG_DV_DEMUXER
    case CODEC_ID_DVAUDIO:
        c->dv_fctx = avformat_alloc_context();
        c->dv_demux = avpriv_dv_init_demux(c->dv_fctx);
        if (!c->dv_demux) {
            av_log(c->fc, AV_LOG_ERROR, "dv demux context init error\n");
            return AVERROR(ENOMEM);
        }
        sc->dv_audio_container = 1;
        st->codec->codec_id = CODEC_ID_PCM_S16LE;
        break;
#endif
     
    case CODEC_ID_QCELP:
        if (st->codec->codec_tag != MKTAG('Q','c','l','p'))
            st->codec->sample_rate = 8000;
        st->codec->channels= 1;  
        break;
    case CODEC_ID_AMR_NB:
        st->codec->channels= 1;  
         
        st->codec->sample_rate = 8000;
        break;
    case CODEC_ID_AMR_WB:
        st->codec->channels    = 1;
        st->codec->sample_rate = 16000;
        break;
    case CODEC_ID_MP2:
    case CODEC_ID_MP3:
        st->codec->codec_type = AVMEDIA_TYPE_AUDIO;  
        st->need_parsing = AVSTREAM_PARSE_FULL;
        break;
    case CODEC_ID_GSM:
    case CODEC_ID_ADPCM_MS:
    case CODEC_ID_ADPCM_IMA_WAV:
        st->codec->block_align = sc->bytes_per_frame;
        break;
    case CODEC_ID_ALAC:
        if (st->codec->extradata_size == 36) {
            st->codec->channels   = AV_RB8 (st->codec->extradata+21);
            st->codec->sample_rate = AV_RB32(st->codec->extradata+32);
        }
        break;
    case CODEC_ID_AC3:
        st->need_parsing = AVSTREAM_PARSE_FULL;
        break;
    case CODEC_ID_MPEG1VIDEO:
        st->need_parsing = AVSTREAM_PARSE_FULL;
        break;
    default:
        break;
    }

    return 0;
}