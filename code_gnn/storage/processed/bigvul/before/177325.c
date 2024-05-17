status_t ACodec::setupVideoEncoder(const char *mime, const sp<AMessage> &msg) {
 int32_t tmp;
 if (!msg->findInt32("color-format", &tmp)) {
 return INVALID_OPERATION;
 }

    OMX_COLOR_FORMATTYPE colorFormat =
 static_cast<OMX_COLOR_FORMATTYPE>(tmp);

 status_t err = setVideoPortFormatType(
            kPortIndexInput, OMX_VIDEO_CodingUnused, colorFormat);

 if (err != OK) {
        ALOGE("[%s] does not support color format %d",
              mComponentName.c_str(), colorFormat);

 return err;
 }

  

    OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);

    OMX_VIDEO_PORTDEFINITIONTYPE *video_def = &def.format.video;

    def.nPortIndex = kPortIndexInput;

    err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err != OK) {
 return err;
 }

 int32_t width, height, bitrate;
 if (!msg->findInt32("width", &width)
 || !msg->findInt32("height", &height)
 || !msg->findInt32("bitrate", &bitrate)) {
 return INVALID_OPERATION;
 }

    video_def->nFrameWidth = width;
    video_def->nFrameHeight = height;

 int32_t stride;
 if (!msg->findInt32("stride", &stride)) {
        stride = width;
 }

    video_def->nStride = stride;

 int32_t sliceHeight;
 if (!msg->findInt32("slice-height", &sliceHeight)) {
        sliceHeight = height;
 }

    video_def->nSliceHeight = sliceHeight;

    def.nBufferSize = (video_def->nStride * video_def->nSliceHeight * 3) / 2;

 float frameRate;
 if (!msg->findFloat("frame-rate", &frameRate)) {
 int32_t tmp;
 if (!msg->findInt32("frame-rate", &tmp)) {
 return INVALID_OPERATION;
 }
        frameRate = (float)tmp;
        mTimePerFrameUs = (int64_t) (1000000.0f / frameRate);
 }

    video_def->xFramerate = (OMX_U32)(frameRate * 65536.0f);
    video_def->eCompressionFormat = OMX_VIDEO_CodingUnused;
 if (colorFormat != OMX_COLOR_FormatYUV420Flexible) {
        video_def->eColorFormat = colorFormat;
 }

    err = mOMX->setParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err != OK) {
        ALOGE("[%s] failed to set input port definition parameters.",
              mComponentName.c_str());

 return err;
 }

  

    OMX_VIDEO_CODINGTYPE compressionFormat;
    err = GetVideoCodingTypeFromMime(mime, &compressionFormat);

 if (err != OK) {
 return err;
 }

    err = setVideoPortFormatType(
            kPortIndexOutput, compressionFormat, OMX_COLOR_FormatUnused);

 if (err != OK) {
        ALOGE("[%s] does not support compression format %d",
             mComponentName.c_str(), compressionFormat);

 return err;
 }

    def.nPortIndex = kPortIndexOutput;

    err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err != OK) {
 return err;
 }

    video_def->nFrameWidth = width;
    video_def->nFrameHeight = height;
    video_def->xFramerate = 0;
    video_def->nBitrate = bitrate;
    video_def->eCompressionFormat = compressionFormat;
    video_def->eColorFormat = OMX_COLOR_FormatUnused;

    err = mOMX->setParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err != OK) {
        ALOGE("[%s] failed to set output port definition parameters.",
              mComponentName.c_str());

 return err;
 }

 switch (compressionFormat) {
 case OMX_VIDEO_CodingMPEG4:
            err = setupMPEG4EncoderParameters(msg);
 break;

 case OMX_VIDEO_CodingH263:
            err = setupH263EncoderParameters(msg);
 break;

 case OMX_VIDEO_CodingAVC:
            err = setupAVCEncoderParameters(msg);
 break;

 case OMX_VIDEO_CodingHEVC:
            err = setupHEVCEncoderParameters(msg);
 break;

 case OMX_VIDEO_CodingVP8:
 case OMX_VIDEO_CodingVP9:
            err = setupVPXEncoderParameters(msg);
 break;

 default:
 break;
 }

 if (err == OK) {
        ALOGI("setupVideoEncoder succeeded");
 }

 return err;
}
