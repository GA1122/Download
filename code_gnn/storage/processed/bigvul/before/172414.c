bool omx_venc::update_profile_level()
{
    OMX_U32 eProfile, eLevel;

 if (!handle->venc_get_profile_level(&eProfile,&eLevel)) {
        DEBUG_PRINT_ERROR("Failed to update the profile_level");
 return false;
 }

    m_sParamProfileLevel.eProfile = (OMX_VIDEO_MPEG4PROFILETYPE)eProfile;
    m_sParamProfileLevel.eLevel = (OMX_VIDEO_MPEG4LEVELTYPE)eLevel;

 if (!strncmp((char *)m_nkind, "OMX.qcom.video.encoder.mpeg4",\
                OMX_MAX_STRINGNAME_SIZE)) {
        m_sParamMPEG4.eProfile = (OMX_VIDEO_MPEG4PROFILETYPE)eProfile;
        m_sParamMPEG4.eLevel = (OMX_VIDEO_MPEG4LEVELTYPE)eLevel;
        DEBUG_PRINT_LOW("MPEG4 profile = %d, level = %d", m_sParamMPEG4.eProfile,
                m_sParamMPEG4.eLevel);
 } else if (!strncmp((char *)m_nkind, "OMX.qcom.video.encoder.h263",\
                OMX_MAX_STRINGNAME_SIZE)) {
        m_sParamH263.eProfile = (OMX_VIDEO_H263PROFILETYPE)eProfile;
        m_sParamH263.eLevel = (OMX_VIDEO_H263LEVELTYPE)eLevel;
        DEBUG_PRINT_LOW("H263 profile = %d, level = %d", m_sParamH263.eProfile,
                m_sParamH263.eLevel);
 } else if (!strncmp((char *)m_nkind, "OMX.qcom.video.encoder.avc",\
                OMX_MAX_STRINGNAME_SIZE)) {
        m_sParamAVC.eProfile = (OMX_VIDEO_AVCPROFILETYPE)eProfile;
        m_sParamAVC.eLevel = (OMX_VIDEO_AVCLEVELTYPE)eLevel;
        DEBUG_PRINT_LOW("AVC profile = %d, level = %d", m_sParamAVC.eProfile,
                m_sParamAVC.eLevel);
 } else if (!strncmp((char *)m_nkind, "OMX.qcom.video.encoder.avc.secure",\
                OMX_MAX_STRINGNAME_SIZE)) {
        m_sParamAVC.eProfile = (OMX_VIDEO_AVCPROFILETYPE)eProfile;
        m_sParamAVC.eLevel = (OMX_VIDEO_AVCLEVELTYPE)eLevel;
        DEBUG_PRINT_LOW("\n AVC profile = %d, level = %d", m_sParamAVC.eProfile,
                m_sParamAVC.eLevel);
 }
 else if (!strncmp((char *)m_nkind, "OMX.qcom.video.encoder.vp8",\
                OMX_MAX_STRINGNAME_SIZE)) {
        m_sParamVP8.eProfile = (OMX_VIDEO_VP8PROFILETYPE)eProfile;
        m_sParamVP8.eLevel = (OMX_VIDEO_VP8LEVELTYPE)eLevel;
        DEBUG_PRINT_LOW("VP8 profile = %d, level = %d", m_sParamVP8.eProfile,
                m_sParamVP8.eLevel);
 }
 else if (!strncmp((char *)m_nkind, "OMX.qcom.video.encoder.hevc",\
                OMX_MAX_STRINGNAME_SIZE)) {
        m_sParamHEVC.eProfile = (OMX_VIDEO_HEVCPROFILETYPE)eProfile;
        m_sParamHEVC.eLevel = (OMX_VIDEO_HEVCLEVELTYPE)eLevel;
        DEBUG_PRINT_LOW("HEVC profile = %d, level = %d", m_sParamHEVC.eProfile,
                m_sParamHEVC.eLevel);
 }

 return true;
}
