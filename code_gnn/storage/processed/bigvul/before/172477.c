bool venc_dev::venc_set_profile_level(OMX_U32 eProfile,OMX_U32 eLevel)
{
 struct venc_profile requested_profile = {0};
 struct ven_profilelevel requested_level = {0};
 unsigned long mb_per_frame = 0;
    DEBUG_PRINT_LOW("venc_set_profile_level:: eProfile = %u, Level = %u",
 (unsigned int)eProfile, (unsigned int)eLevel);
    mb_per_frame = ((m_sVenc_cfg.dvs_height + 15) >> 4)*
 ((m_sVenc_cfg.dvs_width + 15) >> 4);

 if ((eProfile == 0) && (eLevel == 0) && m_profile_set && m_level_set) {
        DEBUG_PRINT_LOW("Profile/Level setting complete before venc_start");
 return true;
 }

    DEBUG_PRINT_LOW("Validating Profile/Level from table");

 if (!venc_validate_profile_level(&eProfile, &eLevel)) {
        DEBUG_PRINT_LOW("ERROR: Profile/Level validation failed");
 return false;
 }

 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_MPEG4) {
        DEBUG_PRINT_LOW("eProfile = %u, OMX_VIDEO_MPEG4ProfileSimple = %d and "
 "OMX_VIDEO_MPEG4ProfileAdvancedSimple = %d", (unsigned int)eProfile,
                OMX_VIDEO_MPEG4ProfileSimple, OMX_VIDEO_MPEG4ProfileAdvancedSimple);

 if (eProfile == OMX_VIDEO_MPEG4ProfileSimple) {
            requested_profile.profile = V4L2_MPEG_VIDEO_MPEG4_PROFILE_SIMPLE;
 } else if (eProfile == OMX_VIDEO_MPEG4ProfileAdvancedSimple) {
            requested_profile.profile = V4L2_MPEG_VIDEO_MPEG4_PROFILE_ADVANCED_SIMPLE;
 } else {
            DEBUG_PRINT_LOW("ERROR: Unsupported MPEG4 profile = %u",
 (unsigned int)eProfile);
 return false;
 }

        DEBUG_PRINT_LOW("eLevel = %u, OMX_VIDEO_MPEG4Level0 = %d, OMX_VIDEO_MPEG4Level1 = %d,"
 "OMX_VIDEO_MPEG4Level2 = %d, OMX_VIDEO_MPEG4Level3 = %d, OMX_VIDEO_MPEG4Level4 = %d,"
 "OMX_VIDEO_MPEG4Level5 = %d", (unsigned int)eLevel, OMX_VIDEO_MPEG4Level0, OMX_VIDEO_MPEG4Level1,
                OMX_VIDEO_MPEG4Level2, OMX_VIDEO_MPEG4Level3, OMX_VIDEO_MPEG4Level4, OMX_VIDEO_MPEG4Level5);

 if (mb_per_frame >= 3600) {
 if (requested_profile.profile == V4L2_MPEG_VIDEO_MPEG4_PROFILE_ADVANCED_SIMPLE)
                requested_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_5;

 if (requested_profile.profile == V4L2_MPEG_VIDEO_MPEG4_PROFILE_SIMPLE)
                requested_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_5;
 } else {
 switch (eLevel) {
 case OMX_VIDEO_MPEG4Level0:
                    requested_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_0;
 break;
 case OMX_VIDEO_MPEG4Level0b:
                    requested_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_0B;
 break;
 case OMX_VIDEO_MPEG4Level1:
                    requested_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_1;
 break;
 case OMX_VIDEO_MPEG4Level2:
                    requested_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_2;
 break;
 case OMX_VIDEO_MPEG4Level3:
                    requested_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_3;
 break;
 case OMX_VIDEO_MPEG4Level4a:
                    requested_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_4;
 break;
 case OMX_VIDEO_MPEG4Level5:
                    requested_level.level = V4L2_MPEG_VIDEO_MPEG4_LEVEL_5;
 break;
 default:
 return false;
 break;
 }
 }
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H263) {

 switch (eProfile) {
 case OMX_VIDEO_H263ProfileBaseline:
                requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_BASELINE;
 break;
 case OMX_VIDEO_H263ProfileH320Coding:
                requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_H320CODING;
 break;
 case OMX_VIDEO_H263ProfileBackwardCompatible:
                requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_BACKWARDCOMPATIBLE;
 break;
 case OMX_VIDEO_H263ProfileISWV2:
                requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_ISWV2;
 break;
 case OMX_VIDEO_H263ProfileISWV3:
                requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_ISWV3;
 break;
 case OMX_VIDEO_H263ProfileHighCompression:
                requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_HIGHCOMPRESSION;
 break;
 case OMX_VIDEO_H263ProfileInternet:
                requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_INTERNET;
 break;
 case OMX_VIDEO_H263ProfileInterlace:
                requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_INTERLACE;
 break;
 case OMX_VIDEO_H263ProfileHighLatency:
                requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_H263_PROFILE_HIGHLATENCY;
 break;
 default:
                DEBUG_PRINT_LOW("ERROR: Unsupported H.263 profile = %lu",
                        requested_profile.profile);
 return false;
 }

 switch (eLevel) {
 case OMX_VIDEO_H263Level10:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_1_0;
 break;
 case OMX_VIDEO_H263Level20:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_2_0;
 break;
 case OMX_VIDEO_H263Level30:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_3_0;
 break;
 case OMX_VIDEO_H263Level40:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_4_0;
 break;
 case OMX_VIDEO_H263Level45:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_4_5;
 break;
 case OMX_VIDEO_H263Level50:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_5_0;
 break;
 case OMX_VIDEO_H263Level60:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_6_0;
 break;
 case OMX_VIDEO_H263Level70:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_H263_LEVEL_7_0;
 break;
 default:
 return false;
 break;
 }
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H264) {
 if (eProfile == OMX_VIDEO_AVCProfileBaseline) {
            requested_profile.profile = V4L2_MPEG_VIDEO_H264_PROFILE_BASELINE;
 } else if(eProfile == QOMX_VIDEO_AVCProfileConstrainedBaseline) {
            requested_profile.profile = V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_BASELINE;
 } else if (eProfile == OMX_VIDEO_AVCProfileMain) {
            requested_profile.profile = V4L2_MPEG_VIDEO_H264_PROFILE_MAIN;
 } else if (eProfile == OMX_VIDEO_AVCProfileExtended) {
            requested_profile.profile = V4L2_MPEG_VIDEO_H264_PROFILE_EXTENDED;
 } else if (eProfile == OMX_VIDEO_AVCProfileHigh) {
            requested_profile.profile = V4L2_MPEG_VIDEO_H264_PROFILE_HIGH;
 } else if (eProfile == OMX_VIDEO_AVCProfileHigh10) {
            requested_profile.profile = V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_10;
 } else if (eProfile == OMX_VIDEO_AVCProfileHigh422) {
            requested_profile.profile = V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_422;
 } else if (eProfile == OMX_VIDEO_AVCProfileHigh444) {
            requested_profile.profile = V4L2_MPEG_VIDEO_H264_PROFILE_HIGH_444_PREDICTIVE;
 } else {
            DEBUG_PRINT_LOW("ERROR: Unsupported H.264 profile = %lu",
                    requested_profile.profile);
 return false;
 }

 switch (eLevel) {
 case OMX_VIDEO_AVCLevel1:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_1_0;
 break;
 case OMX_VIDEO_AVCLevel1b:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_1B;
 break;
 case OMX_VIDEO_AVCLevel11:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_1_1;
 break;
 case OMX_VIDEO_AVCLevel12:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_1_2;
 break;
 case OMX_VIDEO_AVCLevel13:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_1_3;
 break;
 case OMX_VIDEO_AVCLevel2:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_2_0;
 break;
 case OMX_VIDEO_AVCLevel21:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_2_1;
 break;
 case OMX_VIDEO_AVCLevel22:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_2_2;
 break;
 case OMX_VIDEO_AVCLevel3:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_3_0;
 break;
 case OMX_VIDEO_AVCLevel31:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_3_1;
 break;
 case OMX_VIDEO_AVCLevel32:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_3_2;
 break;
 case OMX_VIDEO_AVCLevel4:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_4_0;
 break;
 case OMX_VIDEO_AVCLevel41:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_4_1;
 break;
 case OMX_VIDEO_AVCLevel42:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_4_2;
 break;
 case OMX_VIDEO_AVCLevel5:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_5_0;
 break;
 case OMX_VIDEO_AVCLevel51:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_5_1;
 break;
 case OMX_VIDEO_AVCLevel52:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_5_2;
 break;
 case OMX_VIDEO_AVCLevelMax:
                requested_level.level = V4L2_MPEG_VIDEO_H264_LEVEL_5_2;
 break;
 default :
                DEBUG_PRINT_ERROR("ERROR: Unsupported H.264 level= %lu",
                        requested_level.level);
 return false;
 break;
 }
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_VP8) {
 if (!(eProfile == OMX_VIDEO_VP8ProfileMain)) {
            DEBUG_PRINT_ERROR("ERROR: Unsupported VP8 profile = %u",
 (unsigned int)eProfile);
 return false;
 }
        requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_VP8_UNUSED;
        m_profile_set = true;
 switch(eLevel) {
 case OMX_VIDEO_VP8Level_Version0:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_0;
 break;
 case OMX_VIDEO_VP8Level_Version1:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_1;
 break;
 default:
                DEBUG_PRINT_ERROR("ERROR: Unsupported VP8 level= %u",
 (unsigned int)eLevel);
 return false;
 break;
 }
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_HEVC) {
 if (eProfile == OMX_VIDEO_HEVCProfileMain) {
            requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_HEVC_PROFILE_MAIN;
 } else if(eProfile == OMX_VIDEO_HEVCProfileMain10) {
            requested_profile.profile = V4L2_MPEG_VIDC_VIDEO_HEVC_PROFILE_MAIN10;
 } else {
            DEBUG_PRINT_ERROR("ERROR: Unsupported HEVC profile = %lu",
                    requested_profile.profile);
 return false;
 }

 switch (eLevel) {
 case OMX_VIDEO_HEVCMainTierLevel1:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_1;
 break;
 case OMX_VIDEO_HEVCHighTierLevel1:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_1;
 break;
 case OMX_VIDEO_HEVCMainTierLevel2:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_2;
 break;
 case OMX_VIDEO_HEVCHighTierLevel2:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_2;
 break;
 case OMX_VIDEO_HEVCMainTierLevel21:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_2_1;
 break;
 case OMX_VIDEO_HEVCHighTierLevel21:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_2_1;
 break;
 case OMX_VIDEO_HEVCMainTierLevel3:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_3;
 break;
 case OMX_VIDEO_HEVCHighTierLevel3:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_3;
 break;
 case OMX_VIDEO_HEVCMainTierLevel31:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_3_1;
 break;
 case OMX_VIDEO_HEVCHighTierLevel31:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_3_1;
 break;
 case OMX_VIDEO_HEVCMainTierLevel4:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_4;
 break;
 case OMX_VIDEO_HEVCHighTierLevel4:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_4;
 break;
 case OMX_VIDEO_HEVCMainTierLevel41:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_4_1;
 break;
 case OMX_VIDEO_HEVCHighTierLevel41:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_4_1;
 break;
 case OMX_VIDEO_HEVCMainTierLevel5:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_5;
 break;
 case OMX_VIDEO_HEVCHighTierLevel5:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_5;
 break;
 case OMX_VIDEO_HEVCMainTierLevel51:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_5_1;
 break;
 case OMX_VIDEO_HEVCHighTierLevel51:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_5_1;
 break;
 case OMX_VIDEO_HEVCMainTierLevel52:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_5_2;
 break;
 case OMX_VIDEO_HEVCHighTierLevel52:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_5_2;
 break;
 case OMX_VIDEO_HEVCMainTierLevel6:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_6;
 break;
 case OMX_VIDEO_HEVCHighTierLevel6:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_6;
 break;
 case OMX_VIDEO_HEVCMainTierLevel61:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_6_1;
 break;
 case OMX_VIDEO_HEVCHighTierLevel61:
                requested_level.level = V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_6_1;
 break;
 default :
                DEBUG_PRINT_ERROR("ERROR: Unsupported HEVC level= %lu",
                        requested_level.level);
 return false;
 }
 }

 if (!m_profile_set) {
 int rc;
 struct v4l2_control control;

 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H264) {
            control.id = V4L2_CID_MPEG_VIDEO_H264_PROFILE;
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_MPEG4) {
            control.id = V4L2_CID_MPEG_VIDEO_MPEG4_PROFILE;
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H263) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_H263_PROFILE;
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_HEVC) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_HEVC_PROFILE;
 } else {
            DEBUG_PRINT_ERROR("Wrong CODEC");
 return false;
 }

        control.value = requested_profile.profile;

        DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

        DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);

        codec_profile.profile = control.value;
        m_profile_set = true;
 }

 if (!m_level_set) {
 int rc;
 struct v4l2_control control;

 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H264) {
            control.id = V4L2_CID_MPEG_VIDEO_H264_LEVEL;
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_MPEG4) {
            control.id = V4L2_CID_MPEG_VIDEO_MPEG4_LEVEL;
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H263) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_H263_LEVEL;
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_VP8) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_VP8_PROFILE_LEVEL;
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_HEVC) {
            control.id = V4L2_CID_MPEG_VIDC_VIDEO_HEVC_TIER_LEVEL;
 } else {
            DEBUG_PRINT_ERROR("Wrong CODEC");
 return false;
 }

        control.value = requested_level.level;

        DEBUG_PRINT_LOW("Calling IOCTL set control for id=%d, val=%d", control.id, control.value);
        rc = ioctl(m_nDriver_fd, VIDIOC_S_CTRL, &control);

 if (rc) {
            DEBUG_PRINT_ERROR("Failed to set control");
 return false;
 }

        DEBUG_PRINT_LOW("Success IOCTL set control for id=%d, value=%d", control.id, control.value);

        profile_level.level = control.value;
        m_level_set = true;
 }

 return true;
}
