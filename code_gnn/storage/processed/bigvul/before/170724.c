status_t OMXNodeInstance::createInputSurface(
        OMX_U32 portIndex, sp<IGraphicBufferProducer> *bufferProducer) {
 Mutex::Autolock autolock(mLock);
 status_t err;

 const sp<GraphicBufferSource>& surfaceCheck = getGraphicBufferSource();
 if (surfaceCheck != NULL) {
 return ALREADY_EXISTS;
 }

    OMX_BOOL usingGraphicBuffer = OMX_FALSE;
    err = storeMetaDataInBuffers_l(
            portIndex, OMX_TRUE,
            OMX_TRUE  , &usingGraphicBuffer);
 if (err != OK) {
 return err;
 }

    OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
    def.nPortIndex = portIndex;
    OMX_ERRORTYPE oerr = OMX_GetParameter(
            mHandle, OMX_IndexParamPortDefinition, &def);
 if (oerr != OMX_ErrorNone) {
        OMX_INDEXTYPE index = OMX_IndexParamPortDefinition;
        CLOG_ERROR(getParameter, oerr, "%s(%#x): %s:%u",
                asString(index), index, portString(portIndex), portIndex);
 return UNKNOWN_ERROR;
 }

 if (def.format.video.eColorFormat != OMX_COLOR_FormatAndroidOpaque) {
        CLOGW("createInputSurface requires COLOR_FormatSurface "
 "(AndroidOpaque) color format instead of %s(%#x)",
                asString(def.format.video.eColorFormat), def.format.video.eColorFormat);
 return INVALID_OPERATION;
 }

 GraphicBufferSource* bufferSource = new GraphicBufferSource(
 this, def.format.video.nFrameWidth, def.format.video.nFrameHeight,
            def.nBufferCountActual, usingGraphicBuffer);
 if ((err = bufferSource->initCheck()) != OK) {
 delete bufferSource;
 return err;
 }
    setGraphicBufferSource(bufferSource);

 *bufferProducer = bufferSource->getIGraphicBufferProducer();
 return OK;
}
