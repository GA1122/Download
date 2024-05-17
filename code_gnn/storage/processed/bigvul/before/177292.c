status_t ACodec::requestIDRFrame() {
 if (!mIsEncoder) {
 return ERROR_UNSUPPORTED;
 }

    OMX_CONFIG_INTRAREFRESHVOPTYPE params;
 InitOMXParams(&params);

    params.nPortIndex = kPortIndexOutput;
    params.IntraRefreshVOP = OMX_TRUE;

 return mOMX->setConfig(
            mNode,
            OMX_IndexConfigVideoIntraVOPRefresh,
 &params,
 sizeof(params));
}
