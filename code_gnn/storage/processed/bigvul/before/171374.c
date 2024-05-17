status_t OMXCodec::setVideoOutputFormat(
 const char *mime, const sp<MetaData>& meta) {

 int32_t width, height;
 bool success = meta->findInt32(kKeyWidth, &width);
    success = success && meta->findInt32(kKeyHeight, &height);
    CHECK(success);

    CODEC_LOGV("setVideoOutputFormat width=%d, height=%d", width, height);

    OMX_VIDEO_CODINGTYPE compressionFormat = OMX_VIDEO_CodingUnused;
 if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_AVC, mime)) {
        compressionFormat = OMX_VIDEO_CodingAVC;
 } else if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_MPEG4, mime)) {
        compressionFormat = OMX_VIDEO_CodingMPEG4;
 } else if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_HEVC, mime)) {
        compressionFormat = OMX_VIDEO_CodingHEVC;
 } else if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_H263, mime)) {
        compressionFormat = OMX_VIDEO_CodingH263;
 } else if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_VP8, mime)) {
        compressionFormat = OMX_VIDEO_CodingVP8;
 } else if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_VP9, mime)) {
        compressionFormat = OMX_VIDEO_CodingVP9;
 } else if (!strcasecmp(MEDIA_MIMETYPE_VIDEO_MPEG2, mime)) {
        compressionFormat = OMX_VIDEO_CodingMPEG2;
 } else {
        ALOGE("Not a supported video mime type: %s", mime);
        CHECK(!"Should not be here. Not a supported video mime type.");
 }

 status_t err = setVideoPortFormatType(
            kPortIndexInput, compressionFormat, OMX_COLOR_FormatUnused);

 if (err != OK) {
 return err;
 }

#if 1
 {
        OMX_VIDEO_PARAM_PORTFORMATTYPE format;
 InitOMXParams(&format);
        format.nPortIndex = kPortIndexOutput;
        format.nIndex = 0;

 status_t err = mOMX->getParameter(
                mNode, OMX_IndexParamVideoPortFormat,
 &format, sizeof(format));
        CHECK_EQ(err, (status_t)OK);
        CHECK_EQ((int)format.eCompressionFormat, (int)OMX_VIDEO_CodingUnused);

 int32_t colorFormat;
 if (meta->findInt32(kKeyColorFormat, &colorFormat)
 && colorFormat != OMX_COLOR_FormatUnused
 && colorFormat != format.eColorFormat) {

 while (OMX_ErrorNoMore != err) {
                format.nIndex++;
                err = mOMX->getParameter(
                        mNode, OMX_IndexParamVideoPortFormat,
 &format, sizeof(format));
 if (format.eColorFormat == colorFormat) {
 break;
 }
 }
 if (format.eColorFormat != colorFormat) {
                CODEC_LOGE("Color format %d is not supported", colorFormat);
 return ERROR_UNSUPPORTED;
 }
 }

        err = mOMX->setParameter(
                mNode, OMX_IndexParamVideoPortFormat,
 &format, sizeof(format));

 if (err != OK) {
 return err;
 }
 }
#endif

    OMX_PARAM_PORTDEFINITIONTYPE def;
 InitOMXParams(&def);
    def.nPortIndex = kPortIndexInput;

    OMX_VIDEO_PORTDEFINITIONTYPE *video_def = &def.format.video;

    err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

    CHECK_EQ(err, (status_t)OK);

#if 1
 const size_t X = 64 * 1024;
 if (def.nBufferSize < X) {
        def.nBufferSize = X;
 }
#endif

    CHECK_EQ((int)def.eDomain, (int)OMX_PortDomainVideo);

    video_def->nFrameWidth = width;
    video_def->nFrameHeight = height;

    video_def->eCompressionFormat = compressionFormat;
    video_def->eColorFormat = OMX_COLOR_FormatUnused;

    err = mOMX->setParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 if (err != OK) {
 return err;
 }


 InitOMXParams(&def);
    def.nPortIndex = kPortIndexOutput;

    err = mOMX->getParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));
    CHECK_EQ(err, (status_t)OK);
    CHECK_EQ((int)def.eDomain, (int)OMX_PortDomainVideo);

#if 0
    def.nBufferSize =
 (((width + 15) & -16) * ((height + 15) & -16) * 3) / 2;  
#endif

    video_def->nFrameWidth = width;
    video_def->nFrameHeight = height;

    err = mOMX->setParameter(
            mNode, OMX_IndexParamPortDefinition, &def, sizeof(def));

 return err;
}
