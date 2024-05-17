OMX_ERRORTYPE SoftVPXEncoder::internalSetPortParams(
 const OMX_PARAM_PORTDEFINITIONTYPE* port) {
 if (port->nPortIndex == kInputPortIndex) {
        mWidth = port->format.video.nFrameWidth;
        mHeight = port->format.video.nFrameHeight;

        mFramerate = port->format.video.xFramerate;

 if (port->format.video.eColorFormat == OMX_COLOR_FormatYUV420Planar ||
            port->format.video.eColorFormat == OMX_COLOR_FormatYUV420SemiPlanar ||
            port->format.video.eColorFormat == OMX_COLOR_FormatAndroidOpaque) {
            mColorFormat = port->format.video.eColorFormat;
 } else {
 return OMX_ErrorUnsupportedSetting;
 }

        OMX_PARAM_PORTDEFINITIONTYPE *def = &editPortInfo(kInputPortIndex)->mDef;
        def->format.video.nFrameWidth = mWidth;
        def->format.video.nFrameHeight = mHeight;
        def->format.video.xFramerate = mFramerate;
        def->format.video.eColorFormat = mColorFormat;
        def = &editPortInfo(kOutputPortIndex)->mDef;
        def->format.video.nFrameWidth = mWidth;
        def->format.video.nFrameHeight = mHeight;

 return OMX_ErrorNone;
 } else if (port->nPortIndex == kOutputPortIndex) {
        mBitrate = port->format.video.nBitrate;
        mWidth = port->format.video.nFrameWidth;
        mHeight = port->format.video.nFrameHeight;

        OMX_PARAM_PORTDEFINITIONTYPE *def = &editPortInfo(kOutputPortIndex)->mDef;
        def->format.video.nFrameWidth = mWidth;
        def->format.video.nFrameHeight = mHeight;
        def->format.video.nBitrate = mBitrate;
 return OMX_ErrorNone;
 } else {
 return OMX_ErrorBadPortIndex;
 }
}
