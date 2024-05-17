void SoftVPXEncoder::initPorts() {
    OMX_PARAM_PORTDEFINITIONTYPE inputPort;
    OMX_PARAM_PORTDEFINITIONTYPE outputPort;

 InitOMXParams(&inputPort);
 InitOMXParams(&outputPort);

    inputPort.nBufferCountMin = kNumBuffers;
    inputPort.nBufferCountActual = inputPort.nBufferCountMin;
    inputPort.bEnabled = OMX_TRUE;
    inputPort.bPopulated = OMX_FALSE;
    inputPort.eDomain = OMX_PortDomainVideo;
    inputPort.bBuffersContiguous = OMX_FALSE;
    inputPort.format.video.pNativeRender = NULL;
    inputPort.format.video.nFrameWidth = mWidth;
    inputPort.format.video.nFrameHeight = mHeight;
    inputPort.format.video.nStride = inputPort.format.video.nFrameWidth;
    inputPort.format.video.nSliceHeight = inputPort.format.video.nFrameHeight;
    inputPort.format.video.nBitrate = 0;
    inputPort.format.video.xFramerate = mFramerate;
    inputPort.format.video.bFlagErrorConcealment = OMX_FALSE;
    inputPort.nPortIndex = kInputPortIndex;
    inputPort.eDir = OMX_DirInput;
    inputPort.nBufferAlignment = kInputBufferAlignment;
    inputPort.format.video.cMIMEType =
 const_cast<char *>(MEDIA_MIMETYPE_VIDEO_RAW);
    inputPort.format.video.eCompressionFormat = OMX_VIDEO_CodingUnused;
    inputPort.format.video.eColorFormat = mColorFormat;
    inputPort.format.video.pNativeWindow = NULL;
    inputPort.nBufferSize =
 (inputPort.format.video.nStride *
        inputPort.format.video.nSliceHeight * 3) / 2;

    addPort(inputPort);

    outputPort.nBufferCountMin = kNumBuffers;
    outputPort.nBufferCountActual = outputPort.nBufferCountMin;
    outputPort.bEnabled = OMX_TRUE;
    outputPort.bPopulated = OMX_FALSE;
    outputPort.eDomain = OMX_PortDomainVideo;
    outputPort.bBuffersContiguous = OMX_FALSE;
    outputPort.format.video.pNativeRender = NULL;
    outputPort.format.video.nFrameWidth = mWidth;
    outputPort.format.video.nFrameHeight = mHeight;
    outputPort.format.video.nStride = outputPort.format.video.nFrameWidth;
    outputPort.format.video.nSliceHeight = outputPort.format.video.nFrameHeight;
    outputPort.format.video.nBitrate = mBitrate;
    outputPort.format.video.xFramerate = 0;
    outputPort.format.video.bFlagErrorConcealment = OMX_FALSE;
    outputPort.nPortIndex = kOutputPortIndex;
    outputPort.eDir = OMX_DirOutput;
    outputPort.nBufferAlignment = kOutputBufferAlignment;
    outputPort.format.video.cMIMEType =
 const_cast<char *>(MEDIA_MIMETYPE_VIDEO_VP8);
    outputPort.format.video.eCompressionFormat = OMX_VIDEO_CodingVP8;
    outputPort.format.video.eColorFormat = OMX_COLOR_FormatUnused;
    outputPort.format.video.pNativeWindow = NULL;
    outputPort.nBufferSize = 1024 * 1024;  

    addPort(outputPort);
}
