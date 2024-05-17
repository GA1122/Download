void OMXCodec::setVideoInputFormat(
 const char *mime, const sp<MetaData>& meta) {

 int32_t width, height, frameRate, bitRate, stride, sliceHeight;
 bool success = meta->findInt32(kKeyWidth, &width);
    success = success && meta->findInt32(kKeyHeight, &height);
    success = success && meta->findInt32(kKeyFrameRate, &frameRate);
    success = success && meta->findInt32(kKeyBitRate, &bitRate);
    success = success && meta->findInt32(kKeyStride, &stride);
    success = success && meta->findInt32(kKeySliceHeight, &sliceHeight);
    CHECK(success);
    CHECK(stride != 0);

    OMX_VIDEO_CODINGTYPE compressionFormat = OMX_VIDEO_CodingUnused;
 if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_AVC, mime)) {
        compressionFormat = OMX_VIDEO_CodingAVC;
 } else if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_HEVC, mime)) {
        compressionFormat = OMX_VIDEO_CodingHEVC;
 } else if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_MPEG4, mime)) {
        compressionFormat = OMX_VIDEO_CodingMPEG4;
 } else if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_H263, mime)) {
        compressionFormat = OMX_VIDEO_CodingH263;
 } else {
        ALOGE("Not a supported video mime type: %s", mime);
        CHECK(!"Should not be here. Not a supported video mime type.");
 }

    OMX_COLOR_FORMATTYPE colorFormat;
    CHECK_EQ((status_t)OK, findTargetColorFormat(meta, &colorFormat));

 status_t err;
    OMX_PARAM_PORTDEFINITIONTYPE def;
    OMX_VIDEO_PORTDEFINITIONTYPE *video_def = &def.format.video;

    CHECK_EQ(setVideoPortFormatType(
            kPortIndexInput, OMX_VIDEO_CodingUnused,
            colorFormat), (status_t)OK);

 InitOMXParams(&def);
    def.nPortIndex = kPortIndexInput;

    err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));
    CHECK_EQ(err, (status_t)OK);

    def.nBufferSize = getFrameSize(colorFormat,
            stride > 0? stride: -stride, sliceHeight);

    CHECK_EQ((int)def.eDomain, (int)OMX_PortDomainVideo);

    video_def->nFrameWidth = width;
    video_def->nFrameHeight = height;
    video_def->nStride = stride;
    video_def->nSliceHeight = sliceHeight;
    video_def->xFramerate = (frameRate << 16);  
    video_def->eCompressionFormat = OMX_VIDEO_CodingUnused;
    video_def->eColorFormat = colorFormat;

    err = mOMX->setParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));
    CHECK_EQ(err, (status_t)OK);

    CHECK_EQ(setVideoPortFormatType(
            kPortIndexOutput, compressionFormat, OMX_COLOR_FormatUnused),
 (status_t)OK);
 InitOMXParams(&def);
    def.nPortIndex = kPortIndexOutput;

    err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

    CHECK_EQ(err, (status_t)OK);
    CHECK_EQ((int)def.eDomain, (int)OMX_PortDomainVideo);

    video_def->nFrameWidth = width;
    video_def->nFrameHeight = height;
    video_def->xFramerate = 0;  
    video_def->nBitrate = bitRate;  
    video_def->eCompressionFormat = compressionFormat;
    video_def->eColorFormat = OMX_COLOR_FormatUnused;
 if (mQuirks & kRequiresLargerEncoderOutputBuffer) {
        def.nBufferSize = ((def.nBufferSize * 3) >> 1);
 }

    err = mOMX->setParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));
    CHECK_EQ(err, (status_t)OK);

 switch (compressionFormat) {
 case OMX_VIDEO_CodingMPEG4:
 {
            CHECK_EQ(setupMPEG4EncoderParameters(meta), (status_t)OK);
 break;
 }

 case OMX_VIDEO_CodingH263:
            CHECK_EQ(setupH263EncoderParameters(meta), (status_t)OK);
 break;

 case OMX_VIDEO_CodingAVC:
 {
            CHECK_EQ(setupAVCEncoderParameters(meta), (status_t)OK);
 break;
 }

 default:
            CHECK(!"Support for this compressionFormat to be implemented.");
 break;
 }
}
