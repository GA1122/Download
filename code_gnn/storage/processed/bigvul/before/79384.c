static int mov_write_packet(AVFormatContext *s, AVPacket *pkt)
{
    MOVMuxContext *mov = s->priv_data;
    MOVTrack *trk;

    if (!pkt) {
        mov_flush_fragment(s, 1);
        return 1;
    }

    trk = &mov->tracks[pkt->stream_index];

    if (is_cover_image(trk->st)) {
        int ret;

        if (trk->st->nb_frames >= 1) {
            if (trk->st->nb_frames == 1)
                av_log(s, AV_LOG_WARNING, "Got more than one picture in stream %d,"
                       " ignoring.\n", pkt->stream_index);
            return 0;
        }

        if ((ret = av_packet_ref(&trk->cover_image, pkt)) < 0)
            return ret;

        return 0;
    } else {
        int i;

        if (!pkt->size)
            return mov_write_single_packet(s, pkt);  

         
        for (i = 0; i < mov->nb_streams; i++) {
            MOVTrack *trk = &mov->tracks[i];
            int ret;

            if (trk->par->codec_id == AV_CODEC_ID_MOV_TEXT &&
                trk->track_duration < pkt->dts &&
                (trk->entry == 0 || !trk->last_sample_is_subtitle_end)) {
                ret = mov_write_subtitle_end_packet(s, i, trk->track_duration);
                if (ret < 0) return ret;
                trk->last_sample_is_subtitle_end = 1;
            }
        }

        if (trk->mode == MODE_MOV && trk->par->codec_type == AVMEDIA_TYPE_VIDEO) {
            AVPacket *opkt = pkt;
            int reshuffle_ret, ret;
            if (trk->is_unaligned_qt_rgb) {
                int64_t bpc = trk->par->bits_per_coded_sample != 15 ? trk->par->bits_per_coded_sample : 16;
                int expected_stride = ((trk->par->width * bpc + 15) >> 4)*2;
                reshuffle_ret = ff_reshuffle_raw_rgb(s, &pkt, trk->par, expected_stride);
                if (reshuffle_ret < 0)
                    return reshuffle_ret;
            } else
                reshuffle_ret = 0;
            if (trk->par->format == AV_PIX_FMT_PAL8 && !trk->pal_done) {
                ret = ff_get_packet_palette(s, opkt, reshuffle_ret, trk->palette);
                if (ret < 0)
                    goto fail;
                if (ret)
                    trk->pal_done++;
            } else if (trk->par->codec_id == AV_CODEC_ID_RAWVIDEO &&
                       (trk->par->format == AV_PIX_FMT_GRAY8 ||
                       trk->par->format == AV_PIX_FMT_MONOBLACK)) {
                for (i = 0; i < pkt->size; i++)
                    pkt->data[i] = ~pkt->data[i];
            }
            if (reshuffle_ret) {
                ret = mov_write_single_packet(s, pkt);
fail:
                if (reshuffle_ret)
                    av_packet_free(&pkt);
                return ret;
            }
        }

        return mov_write_single_packet(s, pkt);
    }
}
