status_t OMXNodeInstance::getGraphicBufferUsage(
        OMX_U32 portIndex, OMX_U32* usage) {
 Mutex::Autolock autoLock(mLock);

    OMX_INDEXTYPE index;
    OMX_STRING name = const_cast<OMX_STRING>(
 "OMX.google.android.index.getAndroidNativeBufferUsage");
    OMX_ERRORTYPE err = OMX_GetExtensionIndex(mHandle, name, &index);

 if (err != OMX_ErrorNone) {
        CLOG_ERROR(getExtensionIndex, err, "%s", name);
 return StatusFromOMXError(err);
 }

 GetAndroidNativeBufferUsageParams params;
 InitOMXParams(&params);
    params.nPortIndex = portIndex;

    err = OMX_GetParameter(mHandle, index, &params);
 if (err != OMX_ErrorNone) {
        CLOG_ERROR(getParameter, err, "%s(%#x): %s:%u", name, index,
                portString(portIndex), portIndex);
 return StatusFromOMXError(err);
 }

 *usage = params.nUsage;

 return OK;
}
