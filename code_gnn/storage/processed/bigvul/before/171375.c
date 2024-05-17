status_t OMXCodec::setVideoPortFormatType(
        OMX_U32 portIndex,
        OMX_VIDEO_CODINGTYPE compressionFormat,
        OMX_COLOR_FORMATTYPE colorFormat) {
    OMX_VIDEO_PARAM_PORTFORMATTYPE format;
 InitOMXParams(&format);
    format.nPortIndex = portIndex;
    format.nIndex = 0;
 bool found = false;

    OMX_U32 index = 0;
 for (;;) {
        format.nIndex = index;
 status_t err = mOMX->getParameter(
                mNode, OMX_IndexParamVideoPortFormat,
 &format, sizeof(format));

 if (err != OK) {
 return err;
 }


#if 1
        CODEC_LOGV("portIndex: %u, index: %u, eCompressionFormat=%d eColorFormat=%d",
             portIndex,
             index, format.eCompressionFormat, format.eColorFormat);
#endif

 if (format.eCompressionFormat == compressionFormat
 && format.eColorFormat == colorFormat) {
            found = true;
 break;
 }

 ++index;
 if (index >= kMaxColorFormatSupported) {
            CODEC_LOGE("color format %d or compression format %d is not supported",
                colorFormat, compressionFormat);
 return UNKNOWN_ERROR;
 }
 }

 if (!found) {
 return UNKNOWN_ERROR;
 }

    CODEC_LOGV("found a match.");
 status_t err = mOMX->setParameter(
            mNode, OMX_IndexParamVideoPortFormat,
 &format, sizeof(format));

 return err;
}
