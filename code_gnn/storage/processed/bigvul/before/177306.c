status_t ACodec::setSupportedOutputFormat(bool getLegacyFlexibleFormat) {
    OMX_VIDEO_PARAM_PORTFORMATTYPE format, legacyFormat;
 InitOMXParams(&format);
    format.nPortIndex = kPortIndexOutput;

 InitOMXParams(&legacyFormat);
    legacyFormat.eColorFormat = OMX_COLOR_FormatUnused;

 for (OMX_U32 index = 0; ; ++index) {
        format.nIndex = index;
 status_t err = mOMX->getParameter(
                mNode, OMX_IndexParamVideoPortFormat,
 &format, sizeof(format));
 if (err != OK) {
 if (legacyFormat.eColorFormat != OMX_COLOR_FormatUnused) {
                 memcpy(&format, &legacyFormat, sizeof(format));
 break;
 }
 return err;
 }
 if (format.eCompressionFormat != OMX_VIDEO_CodingUnused) {
 return OMX_ErrorBadParameter;
 }
 if (!getLegacyFlexibleFormat) {
 break;
 }
 if (format.eColorFormat == OMX_COLOR_FormatYUV420Planar
 || format.eColorFormat == OMX_COLOR_FormatYUV420PackedPlanar
 || format.eColorFormat == OMX_COLOR_FormatYUV420SemiPlanar
 || format.eColorFormat == OMX_COLOR_FormatYUV420PackedSemiPlanar
 || format.eColorFormat == OMX_TI_COLOR_FormatYUV420PackedSemiPlanar) {
 break;
 }
        OMX_U32 flexibleEquivalent;
 if (legacyFormat.eColorFormat == OMX_COLOR_FormatUnused
 && isFlexibleColorFormat(
                        mOMX, mNode, format.eColorFormat, false  ,
 &flexibleEquivalent)
 && flexibleEquivalent == OMX_COLOR_FormatYUV420Flexible) {
            memcpy(&legacyFormat, &format, sizeof(format));
 }
 }
 return mOMX->setParameter(
            mNode, OMX_IndexParamVideoPortFormat,
 &format, sizeof(format));
}
