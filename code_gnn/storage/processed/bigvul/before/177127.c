void SoftAVC::initEncParams() {
    mCodecCtx = NULL;
    mMemRecords = NULL;
    mNumMemRecords = DEFAULT_MEM_REC_CNT;
    mHeaderGenerated = 0;
    mNumCores = GetCPUCoreCount();
    mArch = DEFAULT_ARCH;
    mSliceMode = DEFAULT_SLICE_MODE;
    mSliceParam = DEFAULT_SLICE_PARAM;
    mHalfPelEnable = DEFAULT_HPEL;
    mIInterval = DEFAULT_I_INTERVAL;
    mIDRInterval = DEFAULT_IDR_INTERVAL;
    mDisableDeblkLevel = DEFAULT_DISABLE_DEBLK_LEVEL;
    mEnableFastSad = DEFAULT_ENABLE_FAST_SAD;
    mEnableAltRef = DEFAULT_ENABLE_ALT_REF;
    mEncSpeed = DEFAULT_ENC_SPEED;
    mIntra4x4 = DEFAULT_INTRA4x4;
    mAIRMode = DEFAULT_AIR;
    mAIRRefreshPeriod = DEFAULT_AIR_REFRESH_PERIOD;
    mPSNREnable = DEFAULT_PSNR_ENABLE;
    mReconEnable = DEFAULT_RECON_ENABLE;
    mEntropyMode = DEFAULT_ENTROPY_MODE;
    mBframes = DEFAULT_B_FRAMES;

    gettimeofday(&mTimeStart, NULL);
    gettimeofday(&mTimeEnd, NULL);

}
