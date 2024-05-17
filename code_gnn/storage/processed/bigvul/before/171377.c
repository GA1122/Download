status_t OMXCodec::setupBitRate(int32_t bitRate) {
    OMX_VIDEO_PARAM_BITRATETYPE bitrateType;
 InitOMXParams(&bitrateType);
    bitrateType.nPortIndex = kPortIndexOutput;

 status_t err = mOMX->getParameter(
            mNode, OMX_IndexParamVideoBitrate,
 &bitrateType, sizeof(bitrateType));
    CHECK_EQ(err, (status_t)OK);

    bitrateType.eControlRate = OMX_Video_ControlRateVariable;
    bitrateType.nTargetBitrate = bitRate;

    err = mOMX->setParameter(
            mNode, OMX_IndexParamVideoBitrate,
 &bitrateType, sizeof(bitrateType));
    CHECK_EQ(err, (status_t)OK);
 return OK;
}
