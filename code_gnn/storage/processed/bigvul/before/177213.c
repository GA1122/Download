status_t ACodec::configureBitrate(
 int32_t bitrate, OMX_VIDEO_CONTROLRATETYPE bitrateMode) {
    OMX_VIDEO_PARAM_BITRATETYPE bitrateType;
 InitOMXParams(&bitrateType);
    bitrateType.nPortIndex = kPortIndexOutput;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamVideoBitrate,
 &bitrateType, sizeof(bitrateType));

 if (err != OK) {
 return err;
 }

    bitrateType.eControlRate = bitrateMode;
    bitrateType.nTargetBitrate = bitrate;

 return mOMX->setParameter(
            mNode, OMX_IndexParamVideoBitrate,
 &bitrateType, sizeof(bitrateType));
}
