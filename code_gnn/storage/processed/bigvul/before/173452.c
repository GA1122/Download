OMX_ERRORTYPE  omx_vdec::get_component_version
(
 OMX_IN OMX_HANDLETYPE hComp,
 OMX_OUT OMX_STRING componentName,
 OMX_OUT OMX_VERSIONTYPE* componentVersion,
 OMX_OUT OMX_VERSIONTYPE* specVersion,
 OMX_OUT OMX_UUIDTYPE* componentUUID
 )
{
 (void) hComp;
 (void) componentName;
 (void) componentVersion;
 (void) componentUUID;
 if (m_state == OMX_StateInvalid) {
        DEBUG_PRINT_ERROR("Get Comp Version in Invalid State");
 return OMX_ErrorInvalidState;
 }
  
 if (specVersion) {
        specVersion->nVersion = OMX_SPEC_VERSION;
 }
 return OMX_ErrorNone;
}
