OMX_ERRORTYPE SoftAVC::internalSetBitrateParams(
 const OMX_VIDEO_PARAM_BITRATETYPE *bitrate) {
 if (bitrate->nPortIndex != kOutputPortIndex) {
 return OMX_ErrorUnsupportedIndex;
 }

    mBitrate = bitrate->nTargetBitrate;
    mBitrateUpdated = true;

 return OMX_ErrorNone;
}
