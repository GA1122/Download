status_t OMXCodec::isColorFormatSupported(
        OMX_COLOR_FORMATTYPE colorFormat, int portIndex) {
    ALOGV("isColorFormatSupported: %d", static_cast<int>(colorFormat));

    OMX_VIDEO_PARAM_PORTFORMATTYPE portFormat;
 InitOMXParams(&portFormat);
    portFormat.nPortIndex = portIndex;
    OMX_U32 index = 0;
    portFormat.nIndex = index;
 while (true) {
 if (OMX_ErrorNone != mOMX->getParameter(
                mNode, OMX_IndexParamVideoPortFormat,
 &portFormat, sizeof(portFormat))) {
 break;
 }
        CHECK_EQ(index, portFormat.nIndex);
 if (portFormat.eColorFormat == colorFormat) {
            CODEC_LOGV("Found supported color format: %d", portFormat.eColorFormat);
 return OK;  
 }
 ++index;
        portFormat.nIndex = index;

 if (index >= kMaxColorFormatSupported) {
            CODEC_LOGE("More than %u color formats are supported???", index);
 break;
 }
 }

    CODEC_LOGE("color format %d is not supported", colorFormat);
 return UNKNOWN_ERROR;
}
