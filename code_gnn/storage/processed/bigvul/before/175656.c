SoftVPXEncoder::SoftVPXEncoder(const char *name,
 const OMX_CALLBACKTYPE *callbacks,
                               OMX_PTR appData,
                               OMX_COMPONENTTYPE **component)
 : SoftVideoEncoderOMXComponent(name, callbacks, appData, component),
      mCodecContext(NULL),
      mCodecConfiguration(NULL),
      mCodecInterface(NULL),
      mWidth(176),
      mHeight(144),
      mBitrate(192000),  
      mFramerate(30 << 16),  
      mBitrateUpdated(false),
      mBitrateControlMode(VPX_VBR),  
      mDCTPartitions(0),
      mErrorResilience(OMX_FALSE),
      mColorFormat(OMX_COLOR_FormatYUV420Planar),
      mLevel(OMX_VIDEO_VP8Level_Version0),
      mKeyFrameInterval(0),
      mMinQuantizer(0),
      mMaxQuantizer(0),
      mTemporalLayers(0),
      mTemporalPatternType(OMX_VIDEO_VPXTemporalLayerPatternNone),
      mTemporalPatternLength(0),
      mTemporalPatternIdx(0),
      mLastTimestamp(0x7FFFFFFFFFFFFFFFLL),
      mConversionBuffer(NULL),
      mInputDataIsMeta(false),
      mKeyFrameRequested(false) {
    memset(mTemporalLayerBitrateRatio, 0, sizeof(mTemporalLayerBitrateRatio));
    mTemporalLayerBitrateRatio[0] = 100;
    initPorts();
}
