status_t ACodec::setupNativeWindowSizeFormatAndUsage(
 ANativeWindow *nativeWindow  , int *finalUsage  ) {
    OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
    def.nPortIndex = kPortIndexOutput;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err != OK) {
 return err;
 }

    OMX_U32 usage = 0;
    err = mOMX->getGraphicBufferUsage(mNode, kPortIndexOutput, &usage);
 if (err != 0) {
        ALOGW("querying usage flags from OMX IL component failed: %d", err);
        usage = 0;
 }
 int omxUsage = usage;

 if (mFlags & kFlagIsGrallocUsageProtected) {
        usage |= GRALLOC_USAGE_PROTECTED;
 }

    usage |= kVideoGrallocUsage;
 *finalUsage = usage;

    ALOGV("gralloc usage: %#x(OMX) => %#x(ACodec)", omxUsage, usage);
 return setNativeWindowSizeFormatAndUsage(
            nativeWindow,
            def.format.video.nFrameWidth,
            def.format.video.nFrameHeight,
            def.format.video.eColorFormat,
            mRotationDegrees,
            usage);
}
