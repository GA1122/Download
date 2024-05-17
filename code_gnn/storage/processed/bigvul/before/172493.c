bool venc_dev::venc_validate_profile_level(OMX_U32 *eProfile, OMX_U32 *eLevel)
{
    OMX_U32 new_profile = 0, new_level = 0;
 unsigned const int *profile_tbl = NULL;
    OMX_U32 mb_per_frame, mb_per_sec;
 bool profile_level_found = false;

    DEBUG_PRINT_LOW("Init profile table for respective codec");

 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_MPEG4) {
 if (*eProfile == 0) {
 if (!m_profile_set) {
 *eProfile = OMX_VIDEO_MPEG4ProfileSimple;
 } else {
 switch (codec_profile.profile) {
 case V4L2_MPEG_VIDEO_MPEG4_PROFILE_SIMPLE:
 *eProfile = OMX_VIDEO_MPEG4ProfileAdvancedSimple;
 break;
 case V4L2_MPEG_VIDEO_MPEG4_PROFILE_ADVANCED_SIMPLE:
 *eProfile = OMX_VIDEO_MPEG4ProfileSimple;
 break;
 default:
                        DEBUG_PRINT_LOW("%s(): Unknown Error", __func__);
 return false;
 }
 }
 }

 if (*eLevel == 0 && !m_level_set) {
 *eLevel = OMX_VIDEO_MPEG4LevelMax;
 }

 if (*eProfile == OMX_VIDEO_MPEG4ProfileSimple) {
            profile_tbl = (unsigned int const *)mpeg4_profile_level_table;
 } else if (*eProfile == OMX_VIDEO_MPEG4ProfileAdvancedSimple) {
            profile_tbl = (unsigned int const *)
 (&mpeg4_profile_level_table[MPEG4_ASP_START]);
 } else {
            DEBUG_PRINT_LOW("Unsupported MPEG4 profile type %u", (unsigned int)*eProfile);
 return false;
 }
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H264) {
 if (*eProfile == 0) {
 if (!m_profile_set) {
 *eProfile = OMX_VIDEO_AVCProfileBaseline;
 } else {
 switch (codec_profile.profile) {
 case V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE:
 *eProfile = OMX_VIDEO_AVCProfileBaseline;
 break;
 case V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_BASELINE:
 *eProfile = QOMX_VIDEO_AVCProfileConstrainedBaseline;
 break;
 case V4L2_MPEG_VIDEO_H264_PROFILE_MAIN:
 *eProfile = OMX_VIDEO_AVCProfileMain;
 break;
 case V4L2_MPEG_VIDEO_H264_PROFILE_EXTENDED:
 *eProfile = OMX_VIDEO_AVCProfileExtended;
 break;
 case V4L2_MPEG_VIDEO_H264_PROFILE_HIGH:
 *eProfile = OMX_VIDEO_AVCProfileHigh;
 break;
 case V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_10:
 *eProfile = OMX_VIDEO_AVCProfileHigh10;
 break;
 case V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_422:
 *eProfile = OMX_VIDEO_AVCProfileHigh422;
 break;
 case V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_444_PREDICTIVE:
 *eProfile = OMX_VIDEO_AVCProfileHigh444;
 break;
 default:
                        DEBUG_PRINT_LOW("%s(): Unknown Error", __func__);
 return false;
 }
 }
 }

 if (*eLevel == 0 && !m_level_set) {
 *eLevel = OMX_VIDEO_AVCLevelMax;
 }

 if ((*eProfile == OMX_VIDEO_AVCProfileBaseline) ||
 (*eProfile == QOMX_VIDEO_AVCProfileConstrainedBaseline)) {
            profile_tbl = (unsigned int const *)h264_profile_level_table;
 } else if (*eProfile == OMX_VIDEO_AVCProfileHigh) {
            profile_tbl = (unsigned int const *)
 (&h264_profile_level_table[H264_HP_START]);
 } else if (*eProfile == OMX_VIDEO_AVCProfileMain) {
            profile_tbl = (unsigned int const *)
 (&h264_profile_level_table[H264_MP_START]);
 } else {
            DEBUG_PRINT_LOW("Unsupported AVC profile type %u", (unsigned int)*eProfile);
 return false;
 }
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H263) {
 if (*eProfile == 0) {
 if (!m_profile_set) {
 *eProfile = OMX_VIDEO_H263ProfileBaseline;
 } else {
 switch (codec_profile.profile) {
 case VEN_PROFILE_H263_BASELINE:
 *eProfile = OMX_VIDEO_H263ProfileBaseline;
 break;
 default:
                        DEBUG_PRINT_LOW("%s(): Unknown Error", __func__);
 return false;
 }
 }
 }

 if (*eLevel == 0 && !m_level_set) {
 *eLevel = OMX_VIDEO_H263LevelMax;
 }

 if (*eProfile == OMX_VIDEO_H263ProfileBaseline) {
            profile_tbl = (unsigned int const *)h263_profile_level_table;
 } else {
            DEBUG_PRINT_LOW("Unsupported H.263 profile type %u", (unsigned int)*eProfile);
 return false;
 }
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_VP8) {
 if (*eProfile == 0) {
 *eProfile = OMX_VIDEO_VP8ProfileMain;
 } else {
 switch (codec_profile.profile) {
 case V4L2_MPEG_VIDC_VIDEO_VP8_UNUSED:
 *eProfile = OMX_VIDEO_VP8ProfileMain;
 break;
 default:
                    DEBUG_PRINT_ERROR("%s(): Unknown VP8 profile", __func__);
 return false;
 }
 }
 if (*eLevel == 0) {
 switch (profile_level.level) {
 case V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_0:
 *eLevel = OMX_VIDEO_VP8Level_Version0;
 break;
 case V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_1:
 *eLevel = OMX_VIDEO_VP8Level_Version1;
 break;
 default:
                    DEBUG_PRINT_ERROR("%s(): Unknown VP8 level", __func__);
 return false;
 }
 }
 return true;
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_HEVC) {
 if (*eProfile == 0) {
 if (!m_profile_set) {
 *eProfile = OMX_VIDEO_HEVCProfileMain;
 } else {
 switch (codec_profile.profile) {
 case V4L2_MPEG_VIDC_VIDEO_HEVC_PROFILE_MAIN:
 *eProfile = OMX_VIDEO_HEVCProfileMain;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_PROFILE_MAIN10:
 *eProfile = OMX_VIDEO_HEVCProfileMain10;
 break;
 default:
                        DEBUG_PRINT_ERROR("%s(): Unknown Error", __func__);
 return false;
 }
 }
 }

 if (*eLevel == 0 && !m_level_set) {
 *eLevel = OMX_VIDEO_HEVCLevelMax;
 }

 if (*eProfile == OMX_VIDEO_HEVCProfileMain) {
            profile_tbl = (unsigned int const *)hevc_profile_level_table;
 } else if (*eProfile == OMX_VIDEO_HEVCProfileMain10) {
            profile_tbl = (unsigned int const *)
 (&hevc_profile_level_table[HEVC_MAIN10_START]);
 } else {
            DEBUG_PRINT_ERROR("Unsupported HEVC profile type %u", (unsigned int)*eProfile);
 return false;
 }
 } else {
        DEBUG_PRINT_ERROR("Invalid codec type");
 return false;
 }

    mb_per_frame = ((m_sVenc_cfg.dvs_height + 15) >> 4)*
 ((m_sVenc_cfg.dvs_width + 15)>> 4);

 if ((mb_per_frame >= 3600) && (m_sVenc_cfg.codectype == (unsigned long) V4L2_PIX_FMT_MPEG4)) {
 if (codec_profile.profile == (unsigned long) V4L2_MPEG_VIDEO_MPEG4_PROFILE_ADVANCED_SIMPLE)
            profile_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_5;

 if (codec_profile.profile == (unsigned long) V4L2_MPEG_VIDEO_MPEG4_PROFILE_SIMPLE)
            profile_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_5;

 {
            new_level = profile_level.level;
            new_profile = codec_profile.profile;
 return true;
 }
 }

    mb_per_sec = mb_per_frame * m_sVenc_cfg.fps_num / m_sVenc_cfg.fps_den;

 bool h264, ltr, hlayers;
 unsigned int hybridp = 0, maxDpb = profile_tbl[5] / mb_per_frame;
    h264 = m_sVenc_cfg.codectype == V4L2_PIX_FMT_H264;
    ltr = ltrinfo.enabled && ((ltrinfo.count + 2) <= MIN((unsigned int) (profile_tbl[5] / mb_per_frame), MAXDPB));
    hlayers = hier_layers.numlayers && hier_layers.hier_mode == HIER_P &&
 ((intra_period.num_bframes + ltrinfo.count + hier_layers.numlayers + 1) <= (unsigned int) (profile_tbl[5] / profile_tbl[0]));

  

 if(hier_layers.hier_mode == HIER_P_HYBRID)
        hybridp = MIN(MAX(maxDpb, ((hier_layers.numlayers + 1) / 2)), 16);

 do {
 if (mb_per_frame <= (unsigned int)profile_tbl[0]) {
 if (mb_per_sec <= (unsigned int)profile_tbl[1]) {
 if (m_sVenc_cfg.targetbitrate <= (unsigned int)profile_tbl[2]) {
 if (h264 && (ltr || hlayers || hybridp)) {
                        new_level = (int)profile_tbl[3];
                        new_profile = (int)profile_tbl[4];
                        profile_level_found = true;
                        DEBUG_PRINT_LOW("Appropriate profile/level for LTR count: %u OR Hier-p: %u is %u/%u, maxDPB: %u",
                                        ltrinfo.count, hier_layers.numlayers, (int)new_profile, (int)new_level,
                                        MIN((unsigned int) (profile_tbl[5] / mb_per_frame), MAXDPB));
 break;
 } else {
                    new_level = (int)profile_tbl[3];
                    new_profile = (int)profile_tbl[4];
                    profile_level_found = true;
                        DEBUG_PRINT_LOW("Appropriate profile/level found %u/%u", (int) new_profile, (int) new_level);
 break;
 }
 }
 }
 }
        profile_tbl = profile_tbl + MAX_PROFILE_PARAMS;
 } while (profile_tbl[0] != 0);

 if (profile_level_found != true) {
        DEBUG_PRINT_LOW("ERROR: Unsupported profile/level");
 return false;
 }

 if ((*eLevel == OMX_VIDEO_MPEG4LevelMax) || (*eLevel == OMX_VIDEO_AVCLevelMax)
 || (*eLevel == OMX_VIDEO_H263LevelMax) || (*eLevel == OMX_VIDEO_VP8ProfileMax)
 || (*eLevel == OMX_VIDEO_HEVCLevelMax)) {
 *eLevel = new_level;
 }

    DEBUG_PRINT_LOW("%s: Returning with eProfile = %u"
 "Level = %u", __func__, (unsigned int)*eProfile, (unsigned int)*eLevel);

 return true;
}
