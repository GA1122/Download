static int decode_nal_units(HEVCContext *s, const uint8_t *buf, int length)
{
    int i, ret = 0;
    int eos_at_start = 1;

    s->ref = NULL;
    s->last_eos = s->eos;
    s->eos = 0;
    s->overlap = 0;

     
    ret = ff_h2645_packet_split(&s->pkt, buf, length, s->avctx, s->is_nalff,
                                s->nal_length_size, s->avctx->codec_id, 1, 0);
    if (ret < 0) {
        av_log(s->avctx, AV_LOG_ERROR,
               "Error splitting the input into NAL units.\n");
        return ret;
    }

    for (i = 0; i < s->pkt.nb_nals; i++) {
        if (s->pkt.nals[i].type == HEVC_NAL_EOB_NUT ||
            s->pkt.nals[i].type == HEVC_NAL_EOS_NUT) {
            if (eos_at_start) {
                s->last_eos = 1;
            } else {
                s->eos = 1;
            }
        } else {
            eos_at_start = 0;
        }
    }

     
    for (i = 0; i < s->pkt.nb_nals; i++) {
        H2645NAL *nal = &s->pkt.nals[i];

        if (s->avctx->skip_frame >= AVDISCARD_ALL ||
            (s->avctx->skip_frame >= AVDISCARD_NONREF
            && ff_hevc_nal_is_nonref(nal->type)))
            continue;

        ret = decode_nal_unit(s, nal);
        if (ret >= 0 && s->overlap > 2)
            ret = AVERROR_INVALIDDATA;
        if (ret < 0) {
            av_log(s->avctx, AV_LOG_WARNING,
                   "Error parsing NAL unit #%d.\n", i);
            goto fail;
        }
    }

fail:
    if (s->ref && s->threads_type == FF_THREAD_FRAME)
        ff_thread_report_progress(&s->ref->tf, INT_MAX, 0);

    return ret;
}