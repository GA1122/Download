status_t ACodec::setPriority(int32_t priority) {
 if (priority < 0) {
 return BAD_VALUE;
 }
    OMX_PARAM_U32TYPE config;
 InitOMXParams(&config);
    config.nU32 = (OMX_U32)priority;
 status_t temp = mOMX->setConfig(
            mNode, (OMX_INDEXTYPE)OMX_IndexConfigPriority,
 &config, sizeof(config));
 if (temp != OK) {
        ALOGI("codec does not support config priority (err %d)", temp);
 }
 return OK;
}
