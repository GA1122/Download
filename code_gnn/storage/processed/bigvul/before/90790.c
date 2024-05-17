static int set_side_data(HEVCContext *s)
{
    AVFrame *out = s->ref->frame;

    if (s->sei.frame_packing.present &&
        s->sei.frame_packing.arrangement_type >= 3 &&
        s->sei.frame_packing.arrangement_type <= 5 &&
        s->sei.frame_packing.content_interpretation_type > 0 &&
        s->sei.frame_packing.content_interpretation_type < 3) {
        AVStereo3D *stereo = av_stereo3d_create_side_data(out);
        if (!stereo)
            return AVERROR(ENOMEM);

        switch (s->sei.frame_packing.arrangement_type) {
        case 3:
            if (s->sei.frame_packing.quincunx_subsampling)
                stereo->type = AV_STEREO3D_SIDEBYSIDE_QUINCUNX;
            else
                stereo->type = AV_STEREO3D_SIDEBYSIDE;
            break;
        case 4:
            stereo->type = AV_STEREO3D_TOPBOTTOM;
            break;
        case 5:
            stereo->type = AV_STEREO3D_FRAMESEQUENCE;
            break;
        }

        if (s->sei.frame_packing.content_interpretation_type == 2)
            stereo->flags = AV_STEREO3D_FLAG_INVERT;

        if (s->sei.frame_packing.arrangement_type == 5) {
            if (s->sei.frame_packing.current_frame_is_frame0_flag)
                stereo->view = AV_STEREO3D_VIEW_LEFT;
            else
                stereo->view = AV_STEREO3D_VIEW_RIGHT;
        }
    }

    if (s->sei.display_orientation.present &&
        (s->sei.display_orientation.anticlockwise_rotation ||
         s->sei.display_orientation.hflip || s->sei.display_orientation.vflip)) {
        double angle = s->sei.display_orientation.anticlockwise_rotation * 360 / (double) (1 << 16);
        AVFrameSideData *rotation = av_frame_new_side_data(out,
                                                           AV_FRAME_DATA_DISPLAYMATRIX,
                                                           sizeof(int32_t) * 9);
        if (!rotation)
            return AVERROR(ENOMEM);

        av_display_rotation_set((int32_t *)rotation->data, angle);
        av_display_matrix_flip((int32_t *)rotation->data,
                               s->sei.display_orientation.hflip,
                               s->sei.display_orientation.vflip);
    }

    if (s->sei.mastering_display.present > 0 &&
        IS_IRAP(s) && s->no_rasl_output_flag) {
        s->sei.mastering_display.present--;
    }
    if (s->sei.mastering_display.present) {
        const int mapping[3] = {2, 0, 1};
        const int chroma_den = 50000;
        const int luma_den = 10000;
        int i;
        AVMasteringDisplayMetadata *metadata =
            av_mastering_display_metadata_create_side_data(out);
        if (!metadata)
            return AVERROR(ENOMEM);

        for (i = 0; i < 3; i++) {
            const int j = mapping[i];
            metadata->display_primaries[i][0].num = s->sei.mastering_display.display_primaries[j][0];
            metadata->display_primaries[i][0].den = chroma_den;
            metadata->display_primaries[i][1].num = s->sei.mastering_display.display_primaries[j][1];
            metadata->display_primaries[i][1].den = chroma_den;
        }
        metadata->white_point[0].num = s->sei.mastering_display.white_point[0];
        metadata->white_point[0].den = chroma_den;
        metadata->white_point[1].num = s->sei.mastering_display.white_point[1];
        metadata->white_point[1].den = chroma_den;

        metadata->max_luminance.num = s->sei.mastering_display.max_luminance;
        metadata->max_luminance.den = luma_den;
        metadata->min_luminance.num = s->sei.mastering_display.min_luminance;
        metadata->min_luminance.den = luma_den;
        metadata->has_luminance = 1;
        metadata->has_primaries = 1;

        av_log(s->avctx, AV_LOG_DEBUG, "Mastering Display Metadata:\n");
        av_log(s->avctx, AV_LOG_DEBUG,
               "r(%5.4f,%5.4f) g(%5.4f,%5.4f) b(%5.4f %5.4f) wp(%5.4f, %5.4f)\n",
               av_q2d(metadata->display_primaries[0][0]),
               av_q2d(metadata->display_primaries[0][1]),
               av_q2d(metadata->display_primaries[1][0]),
               av_q2d(metadata->display_primaries[1][1]),
               av_q2d(metadata->display_primaries[2][0]),
               av_q2d(metadata->display_primaries[2][1]),
               av_q2d(metadata->white_point[0]), av_q2d(metadata->white_point[1]));
        av_log(s->avctx, AV_LOG_DEBUG,
               "min_luminance=%f, max_luminance=%f\n",
               av_q2d(metadata->min_luminance), av_q2d(metadata->max_luminance));
    }
    if (s->sei.content_light.present > 0 &&
        IS_IRAP(s) && s->no_rasl_output_flag) {
        s->sei.content_light.present--;
    }
    if (s->sei.content_light.present) {
        AVContentLightMetadata *metadata =
            av_content_light_metadata_create_side_data(out);
        if (!metadata)
            return AVERROR(ENOMEM);
        metadata->MaxCLL  = s->sei.content_light.max_content_light_level;
        metadata->MaxFALL = s->sei.content_light.max_pic_average_light_level;

        av_log(s->avctx, AV_LOG_DEBUG, "Content Light Level Metadata:\n");
        av_log(s->avctx, AV_LOG_DEBUG, "MaxCLL=%d, MaxFALL=%d\n",
               metadata->MaxCLL, metadata->MaxFALL);
    }

    if (s->sei.a53_caption.a53_caption) {
        AVFrameSideData* sd = av_frame_new_side_data(out,
                                                     AV_FRAME_DATA_A53_CC,
                                                     s->sei.a53_caption.a53_caption_size);
        if (sd)
            memcpy(sd->data, s->sei.a53_caption.a53_caption, s->sei.a53_caption.a53_caption_size);
        av_freep(&s->sei.a53_caption.a53_caption);
        s->sei.a53_caption.a53_caption_size = 0;
        s->avctx->properties |= FF_CODEC_PROPERTY_CLOSED_CAPTIONS;
    }

    if (s->sei.alternative_transfer.present &&
        av_color_transfer_name(s->sei.alternative_transfer.preferred_transfer_characteristics) &&
        s->sei.alternative_transfer.preferred_transfer_characteristics != AVCOL_TRC_UNSPECIFIED) {
        s->avctx->color_trc = out->color_trc = s->sei.alternative_transfer.preferred_transfer_characteristics;
    }

    return 0;
}