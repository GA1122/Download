status_t SoftAMRWBEncoder::initEncoder() {
    mApiHandle = new VO_AUDIO_CODECAPI;

 if (VO_ERR_NONE != voGetAMRWBEncAPI(mApiHandle)) {
        ALOGE("Failed to get api handle");
 return UNKNOWN_ERROR;
 }

    mMemOperator = new VO_MEM_OPERATOR;
    mMemOperator->Alloc = cmnMemAlloc;
    mMemOperator->Copy = cmnMemCopy;
    mMemOperator->Free = cmnMemFree;
    mMemOperator->Set = cmnMemSet;
    mMemOperator->Check = cmnMemCheck;

    VO_CODEC_INIT_USERDATA userData;
    memset(&userData, 0, sizeof(userData));
    userData.memflag = VO_IMF_USERMEMOPERATOR;
    userData.memData = (VO_PTR) mMemOperator;

 if (VO_ERR_NONE != mApiHandle->Init(
 &mEncoderHandle, VO_AUDIO_CodingAMRWB, &userData)) {
        ALOGE("Failed to init AMRWB encoder");
 return UNKNOWN_ERROR;
 }

    VOAMRWBFRAMETYPE type = VOAMRWB_RFC3267;
 if (VO_ERR_NONE != mApiHandle->SetParam(
                mEncoderHandle, VO_PID_AMRWB_FRAMETYPE, &type)) {
        ALOGE("Failed to set AMRWB encoder frame type to %d", type);
 return UNKNOWN_ERROR;
 }

 return OK;
}
