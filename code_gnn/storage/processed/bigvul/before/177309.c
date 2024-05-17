status_t ACodec::setVideoPortFormatType(
        OMX_U32 portIndex,
        OMX_VIDEO_CODINGTYPE compressionFormat,
        OMX_COLOR_FORMATTYPE colorFormat,
 bool usingNativeBuffers) {
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

        OMX_U32 flexibleEquivalent;
 if (compressionFormat == OMX_VIDEO_CodingUnused
 && isFlexibleColorFormat(
                        mOMX, mNode, format.eColorFormat, usingNativeBuffers, &flexibleEquivalent)
 && colorFormat == flexibleEquivalent) {
            ALOGI("[%s] using color format %#x in place of %#x",
                    mComponentName.c_str(), format.eColorFormat, colorFormat);
            colorFormat = format.eColorFormat;
 }


 if (!strcmp("OMX.TI.Video.encoder", mComponentName.c_str())) {
 if (portIndex == kPortIndexInput
 && colorFormat == format.eColorFormat) {
                found = true;
 break;
 }
 if (portIndex == kPortIndexOutput
 && compressionFormat == format.eCompressionFormat) {
                found = true;
 break;
 }
 }

 if (format.eCompressionFormat == compressionFormat
 && format.eColorFormat == colorFormat) {
            found = true;
 break;
 }

 ++index;
 }

 if (!found) {
 return UNKNOWN_ERROR;
 }

 status_t err = mOMX->setParameter(
            mNode, OMX_IndexParamVideoPortFormat,
 &format, sizeof(format));

 return err;
}
