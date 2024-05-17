OMX_ERRORTYPE SoftVPXEncoder::internalSetRoleParams(
 const OMX_PARAM_COMPONENTROLETYPE* role) {
 const char* roleText = (const char*)role->cRole;
 const size_t roleTextMaxSize = OMX_MAX_STRINGNAME_SIZE - 1;

 if (strncmp(roleText, "video_encoder.vp8", roleTextMaxSize)) {
        ALOGE("Unsupported component role");
 return OMX_ErrorBadParameter;
 }

 return OMX_ErrorNone;
}
