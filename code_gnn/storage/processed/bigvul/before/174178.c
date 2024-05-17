SoftVPXEncoder::SoftVPXEncoder(const char *name,
 const OMX_CALLBACKTYPE *callbacks,
                               OMX_PTR appData,
                               OMX_COMPONENTTYPE **component)
 : SoftVideoEncoderOMXComponent(
            name, "video_encoder.vp8", OMX_VIDEO_CodingVP8,
            kProfileLevels, NELEM(kProfileLevels),
 176  , 144  ,
            callbacks, appData, component),
      mCodecContext(NULL),
      mCodecConfiguration(NULL),
      mCodecInterface(NULL),
      mBitrateUpdated(false),
      mBitrateControlMode(VPX_VBR),  
      mDCTPartitions(0),
      mErrorResilience(OMX_FALSE),
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
      mKeyFrameRequested(false) {
    memset(mTemporalLayerBitrateRatio, 0, sizeof(mTemporalLayerBitrateRatio));
    mTemporalLayerBitrateRatio[0] = 100;

 const size_t kMinOutputBufferSize = 1024 * 1024;  

    initPorts(
            kNumBuffers, kNumBuffers, kMinOutputBufferSize,
            MEDIA_MIMETYPE_VIDEO_VP8, 2  );
}
