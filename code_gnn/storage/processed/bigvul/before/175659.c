OMX_ERRORTYPE SoftVPXEncoder::internalSetFormatParams(
 const OMX_VIDEO_PARAM_PORTFORMATTYPE* format) {
 if (format->nPortIndex == kInputPortIndex) {
 if (format->eColorFormat == OMX_COLOR_FormatYUV420Planar ||
            format->eColorFormat == OMX_COLOR_FormatYUV420SemiPlanar ||
            format->eColorFormat == OMX_COLOR_FormatAndroidOpaque) {
            mColorFormat = format->eColorFormat;

            OMX_PARAM_PORTDEFINITIONTYPE *def = &editPortInfo(kInputPortIndex)->mDef;
            def->format.video.eColorFormat = mColorFormat;

 return OMX_ErrorNone;
 } else {
            ALOGE("Unsupported color format %i", format->eColorFormat);
 return OMX_ErrorUnsupportedSetting;
 }
 } else if (format->nPortIndex == kOutputPortIndex) {
 if (format->eCompressionFormat == OMX_VIDEO_CodingVP8) {
 return OMX_ErrorNone;
 } else {
 return OMX_ErrorUnsupportedSetting;
 }
 } else {
 return OMX_ErrorBadPortIndex;
 }
}
