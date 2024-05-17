status_t ACodec::setVideoFormatOnPort(
        OMX_U32 portIndex,
 int32_t width, int32_t height, OMX_VIDEO_CODINGTYPE compressionFormat,
 float frameRate) {
    OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
    def.nPortIndex = portIndex;

    OMX_VIDEO_PORTDEFINITIONTYPE *video_def = &def.format.video;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));
 if (err != OK) {
 return err;
 }

 if (portIndex == kPortIndexInput) {
 const size_t X = 64 * 1024;
 if (def.nBufferSize < X) {
            def.nBufferSize = X;
 }
 }

 if (def.eDomain != OMX_PortDomainVideo) {
        ALOGE("expected video port, got %s(%d)", asString(def.eDomain), def.eDomain);
 return FAILED_TRANSACTION;
 }

    video_def->nFrameWidth = width;
    video_def->nFrameHeight = height;

 if (portIndex == kPortIndexInput) {
        video_def->eCompressionFormat = compressionFormat;
        video_def->eColorFormat = OMX_COLOR_FormatUnused;
 if (frameRate >= 0) {
            video_def->xFramerate = (OMX_U32)(frameRate * 65536.0f);
 }
 }

    err = mOMX->setParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 return err;
}
