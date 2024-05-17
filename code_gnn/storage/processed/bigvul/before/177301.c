status_t ACodec::setOperatingRate(float rateFloat, bool isVideo) {
 if (rateFloat < 0) {
 return BAD_VALUE;
 }
    OMX_U32 rate;
 if (isVideo) {
 if (rateFloat > 65535) {
 return BAD_VALUE;
 }
        rate = (OMX_U32)(rateFloat * 65536.0f + 0.5f);
 } else {
 if (rateFloat > UINT_MAX) {
 return BAD_VALUE;
 }
        rate = (OMX_U32)(rateFloat);
 }
    OMX_PARAM_U32TYPE config;
 InitOMXParams(&config);
    config.nU32 = rate;
 status_t err = mOMX->setConfig(
            mNode, (OMX_INDEXTYPE)OMX_IndexConfigOperatingRate,
 &config, sizeof(config));
 if (err != OK) {
        ALOGI("codec does not support config operating rate (err %d)", err);
 }
 return OK;
}
