OMX_ERRORTYPE  omx_video::component_role_enum(OMX_IN OMX_HANDLETYPE hComp,
        OMX_OUT OMX_U8*        role,
        OMX_IN OMX_U32        index)
{
 (void)hComp;
    OMX_ERRORTYPE eRet = OMX_ErrorNone;
 if (!strncmp((char*)m_nkind, "OMX.qcom.video.decoder.mpeg4",OMX_MAX_STRINGNAME_SIZE)) {
 if ((0 == index) && role) {
            strlcpy((char *)role, "video_decoder.mpeg4",OMX_MAX_STRINGNAME_SIZE);
            DEBUG_PRINT_LOW("component_role_enum: role %s",role);
 } else {
            eRet = OMX_ErrorNoMore;
 }
 } else if (!strncmp((char*)m_nkind, "OMX.qcom.video.decoder.h263",OMX_MAX_STRINGNAME_SIZE)) {
 if ((0 == index) && role) {
            strlcpy((char *)role, "video_decoder.h263",OMX_MAX_STRINGNAME_SIZE);
            DEBUG_PRINT_LOW("component_role_enum: role %s",role);
 } else {
            DEBUG_PRINT_ERROR("ERROR: No more roles");
            eRet = OMX_ErrorNoMore;
 }
 } else if (!strncmp((char*)m_nkind, "OMX.qcom.video.decoder.avc",OMX_MAX_STRINGNAME_SIZE)) {
 if ((0 == index) && role) {
            strlcpy((char *)role, "video_decoder.avc",OMX_MAX_STRINGNAME_SIZE);
            DEBUG_PRINT_LOW("component_role_enum: role %s",role);
 } else {
            DEBUG_PRINT_ERROR("ERROR: No more roles");
            eRet = OMX_ErrorNoMore;
 }
 } else if (!strncmp((char*)m_nkind, "OMX.qcom.video.decoder.vc1",OMX_MAX_STRINGNAME_SIZE)) {
 if ((0 == index) && role) {
            strlcpy((char *)role, "video_decoder.vc1",OMX_MAX_STRINGNAME_SIZE);
            DEBUG_PRINT_LOW("component_role_enum: role %s",role);
 } else {
            DEBUG_PRINT_ERROR("ERROR: No more roles");
            eRet = OMX_ErrorNoMore;
 }
 }
 if (!strncmp((char*)m_nkind, "OMX.qcom.video.encoder.mpeg4",OMX_MAX_STRINGNAME_SIZE)) {
 if ((0 == index) && role) {
            strlcpy((char *)role, "video_encoder.mpeg4",OMX_MAX_STRINGNAME_SIZE);
            DEBUG_PRINT_LOW("component_role_enum: role %s",role);
 } else {
            eRet = OMX_ErrorNoMore;
 }
 } else if (!strncmp((char*)m_nkind, "OMX.qcom.video.encoder.h263",OMX_MAX_STRINGNAME_SIZE)) {
 if ((0 == index) && role) {
            strlcpy((char *)role, "video_encoder.h263",OMX_MAX_STRINGNAME_SIZE);
            DEBUG_PRINT_LOW("component_role_enum: role %s",role);
 } else {
            DEBUG_PRINT_ERROR("ERROR: No more roles");
            eRet = OMX_ErrorNoMore;
 }
 } else if (!strncmp((char*)m_nkind, "OMX.qcom.video.encoder.avc",OMX_MAX_STRINGNAME_SIZE)) {
 if ((0 == index) && role) {
            strlcpy((char *)role, "video_encoder.avc",OMX_MAX_STRINGNAME_SIZE);
            DEBUG_PRINT_LOW("component_role_enum: role %s",role);
 } else {
            DEBUG_PRINT_ERROR("ERROR: No more roles");
            eRet = OMX_ErrorNoMore;
 }
 }
#ifdef _MSM8974_
 else if (!strncmp((char*)m_nkind, "OMX.qcom.video.encoder.vp8",OMX_MAX_STRINGNAME_SIZE)) {
 if ((0 == index) && role) {
            strlcpy((char *)role, "video_encoder.vp8",OMX_MAX_STRINGNAME_SIZE);
            DEBUG_PRINT_LOW("component_role_enum: role %s",role);
 } else {
            DEBUG_PRINT_ERROR("ERROR: No more roles");
            eRet = OMX_ErrorNoMore;
 }
 }
#endif
 else if ((!strncmp((char*)m_nkind, "OMX.qcom.video.encoder.hevc", OMX_MAX_STRINGNAME_SIZE)) ||
 (!strncmp((char*)m_nkind, "OMX.qti.video.encoder.hevc", OMX_MAX_STRINGNAME_SIZE))) {
 if ((0 == index) && role) {
            strlcpy((char *)role, "video_encoder.hevc", OMX_MAX_STRINGNAME_SIZE);
            DEBUG_PRINT_LOW("component_role_enum: role %s", role);
 } else {
            DEBUG_PRINT_ERROR("ERROR: No more roles");
            eRet = OMX_ErrorNoMore;
 }
 }
 else {
        DEBUG_PRINT_ERROR("ERROR: Querying Role on Unknown Component");
        eRet = OMX_ErrorInvalidComponentName;
 }
 return eRet;
}
