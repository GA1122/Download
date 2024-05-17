bool venc_dev::venc_get_profile_level(OMX_U32 *eProfile,OMX_U32 *eLevel)
{
 bool status = true;

 if (eProfile == NULL || eLevel == NULL) {
 return false;
 }

 if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_MPEG4) {
 switch (codec_profile.profile) {
 case V4L2_MPEG_VIDEO_MPEG4_PROFILE_SIMPLE:
 *eProfile = OMX_VIDEO_MPEG4ProfileSimple;
 break;
 case V4L2_MPEG_VIDEO_MPEG4_PROFILE_ADVANCED_SIMPLE:
 *eProfile = OMX_VIDEO_MPEG4ProfileAdvancedSimple;
 break;
 default:
 *eProfile = OMX_VIDEO_MPEG4ProfileMax;
                status = false;
 break;
 }

 if (!status) {
 return status;
 }

 switch (profile_level.level) {
 case V4L2_MPEG_VIDEO_MPEG4_LEVEL_0:
 *eLevel = OMX_VIDEO_MPEG4Level0;
 break;
 case V4L2_MPEG_VIDEO_MPEG4_LEVEL_0B:
 *eLevel = OMX_VIDEO_MPEG4Level0b;
 break;
 case V4L2_MPEG_VIDEO_MPEG4_LEVEL_1:
 *eLevel = OMX_VIDEO_MPEG4Level1;
 break;
 case V4L2_MPEG_VIDEO_MPEG4_LEVEL_2:
 *eLevel = OMX_VIDEO_MPEG4Level2;
 break;
 case V4L2_MPEG_VIDEO_MPEG4_LEVEL_3:
 *eLevel = OMX_VIDEO_MPEG4Level3;
 break;
 case V4L2_MPEG_VIDEO_MPEG4_LEVEL_4:
 *eLevel = OMX_VIDEO_MPEG4Level4;
 break;
 case V4L2_MPEG_VIDEO_MPEG4_LEVEL_5:
 *eLevel = OMX_VIDEO_MPEG4Level5;
 break;
 default:
 *eLevel = OMX_VIDEO_MPEG4LevelMax;
                status = false;
 break;
 }
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H263) {
 if (codec_profile.profile == VEN_PROFILE_H263_BASELINE) {
 *eProfile = OMX_VIDEO_H263ProfileBaseline;
 } else {
 *eProfile = OMX_VIDEO_H263ProfileMax;
 return false;
 }

 switch (profile_level.level) {
 case VEN_LEVEL_H263_10:
 *eLevel = OMX_VIDEO_H263Level10;
 break;
 case VEN_LEVEL_H263_20:
 *eLevel = OMX_VIDEO_H263Level20;
 break;
 case VEN_LEVEL_H263_30:
 *eLevel = OMX_VIDEO_H263Level30;
 break;
 case VEN_LEVEL_H263_40:
 *eLevel = OMX_VIDEO_H263Level40;
 break;
 case VEN_LEVEL_H263_45:
 *eLevel = OMX_VIDEO_H263Level45;
 break;
 case VEN_LEVEL_H263_50:
 *eLevel = OMX_VIDEO_H263Level50;
 break;
 case VEN_LEVEL_H263_60:
 *eLevel = OMX_VIDEO_H263Level60;
 break;
 case VEN_LEVEL_H263_70:
 *eLevel = OMX_VIDEO_H263Level70;
 break;
 default:
 *eLevel = OMX_VIDEO_H263LevelMax;
                status = false;
 break;
 }
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_H264) {
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
 case V4L2_MPEG_VIDEO_H264_PROFILE_HIGH:
 *eProfile = OMX_VIDEO_AVCProfileHigh;
 break;
 case V4L2_MPEG_VIDEO_H264_PROFILE_EXTENDED:
 *eProfile = OMX_VIDEO_AVCProfileExtended;
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
 *eProfile = OMX_VIDEO_AVCProfileMax;
                status = false;
 break;
 }

 if (!status) {
 return status;
 }

 switch (profile_level.level) {
 case V4L2_MPEG_VIDEO_H264_LEVEL_1_0:
 *eLevel = OMX_VIDEO_AVCLevel1;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_1B:
 *eLevel = OMX_VIDEO_AVCLevel1b;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_1_1:
 *eLevel = OMX_VIDEO_AVCLevel11;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_1_2:
 *eLevel = OMX_VIDEO_AVCLevel12;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_1_3:
 *eLevel = OMX_VIDEO_AVCLevel13;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_2_0:
 *eLevel = OMX_VIDEO_AVCLevel2;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_2_1:
 *eLevel = OMX_VIDEO_AVCLevel21;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_2_2:
 *eLevel = OMX_VIDEO_AVCLevel22;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_3_0:
 *eLevel = OMX_VIDEO_AVCLevel3;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_3_1:
 *eLevel = OMX_VIDEO_AVCLevel31;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_3_2:
 *eLevel = OMX_VIDEO_AVCLevel32;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_4_0:
 *eLevel = OMX_VIDEO_AVCLevel4;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_4_1:
 *eLevel = OMX_VIDEO_AVCLevel41;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_4_2:
 *eLevel = OMX_VIDEO_AVCLevel42;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_5_0:
 *eLevel = OMX_VIDEO_AVCLevel5;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_5_1:
 *eLevel = OMX_VIDEO_AVCLevel51;
 break;
 case V4L2_MPEG_VIDEO_H264_LEVEL_5_2:
 *eLevel = OMX_VIDEO_AVCLevel52;
 break;
 default :
 *eLevel = OMX_VIDEO_AVCLevelMax;
                status = false;
 break;
 }
 }
 else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_VP8) {
 switch (codec_profile.profile) {
 case V4L2_MPEG_VIDC_VIDEO_VP8_UNUSED:
 *eProfile = OMX_VIDEO_VP8ProfileMain;
 break;
 default:
 *eProfile = OMX_VIDEO_VP8ProfileMax;
                status = false;
 break;
 }
 if (!status) {
 return status;
 }

 switch (profile_level.level) {
 case V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_0:
 *eLevel = OMX_VIDEO_VP8Level_Version0;
 break;
 case V4L2_MPEG_VIDC_VIDEO_VP8_VERSION_1:
 *eLevel = OMX_VIDEO_VP8Level_Version1;
 break;
 default:
 *eLevel = OMX_VIDEO_VP8LevelMax;
                status = false;
 break;
 }
 } else if (m_sVenc_cfg.codectype == V4L2_PIX_FMT_HEVC) {
 switch (codec_profile.profile) {
 case V4L2_MPEG_VIDC_VIDEO_HEVC_PROFILE_MAIN:
 *eProfile = OMX_VIDEO_HEVCProfileMain;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_PROFILE_MAIN10:
 *eProfile = OMX_VIDEO_HEVCProfileMain10;
 break;
 default:
 *eProfile = OMX_VIDEO_HEVCProfileMax;
                status = false;
 break;
 }
 if (!status) {
 return status;
 }

 switch (profile_level.level) {
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_1:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel1;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_1:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel1;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_2:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel2;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_2:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel2;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_2_1:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel21;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_2_1:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel21;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_3:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel3;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_3:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel3;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_3_1:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel31;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_3_1:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel31;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_4:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel4;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_4:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel4;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_4_1:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel41;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_4_1:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel41;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_5:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel5;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_5:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel5;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_5_1:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel51;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_5_1:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel51;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_5_2:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel52;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_5_2:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel52;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_6:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel6;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_6:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel6;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_6_1:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel61;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_HIGH_TIER_LEVEL_6_1:
 *eLevel = OMX_VIDEO_HEVCHighTierLevel61;
 break;
 case V4L2_MPEG_VIDC_VIDEO_HEVC_LEVEL_MAIN_TIER_LEVEL_6_2:
 *eLevel = OMX_VIDEO_HEVCMainTierLevel62;
 break;
 default:
 *eLevel = OMX_VIDEO_HEVCLevelMax;
                status = false;
 break;
 }
 }

 return status;
}
