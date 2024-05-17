status_t OMXNodeInstance::storeMetaDataInBuffers_l(
        OMX_U32 portIndex,
        OMX_BOOL enable,
        OMX_BOOL useGraphicBuffer,
        OMX_BOOL *usingGraphicBufferInMetadata) {
    OMX_INDEXTYPE index;
    OMX_STRING name = const_cast<OMX_STRING>(
 "OMX.google.android.index.storeMetaDataInBuffers");

    OMX_STRING graphicBufferName = const_cast<OMX_STRING>(
 "OMX.google.android.index.storeGraphicBufferInMetaData");
 if (usingGraphicBufferInMetadata == NULL) {
        usingGraphicBufferInMetadata = &useGraphicBuffer;
 }

    OMX_ERRORTYPE err =
 (useGraphicBuffer && portIndex == kPortIndexInput)
 ? OMX_GetExtensionIndex(mHandle, graphicBufferName, &index)
 : OMX_ErrorBadParameter;
 if (err == OMX_ErrorNone) {
 *usingGraphicBufferInMetadata = OMX_TRUE;
        name = graphicBufferName;
 } else {
        err = OMX_GetExtensionIndex(mHandle, name, &index);
 }

    OMX_ERRORTYPE xerr = err;
 if (err == OMX_ErrorNone) {
 StoreMetaDataInBuffersParams params;
 InitOMXParams(&params);
        params.nPortIndex = portIndex;
        params.bStoreMetaData = enable;

        err = OMX_SetParameter(mHandle, index, &params);
 }

 if (err != OMX_ErrorNone) {
 *usingGraphicBufferInMetadata = OMX_FALSE;
 if (err == OMX_ErrorUnsupportedIndex && portIndex == kPortIndexOutput) {
            CLOGW("component does not support metadata mode; using fallback");
 } else if (xerr != OMX_ErrorNone) {
            CLOG_ERROR(getExtensionIndex, xerr, "%s", name);
 } else {
            CLOG_ERROR(setParameter, err, "%s(%#x): %s:%u en=%d GB=%d", name, index,
                    portString(portIndex), portIndex, enable, useGraphicBuffer);
 }
 }
 return StatusFromOMXError(err);
}
