status_t StatusFromOMXError(OMX_ERRORTYPE err) {
 switch (err) {
 case OMX_ErrorNone:
 return OK;
 case OMX_ErrorUnsupportedSetting:
 case OMX_ErrorUnsupportedIndex:
 return ERROR_UNSUPPORTED;
 case OMX_ErrorInsufficientResources:
 return NO_MEMORY;
 default:
 return UNKNOWN_ERROR;
 }
}
