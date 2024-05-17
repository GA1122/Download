OMX_ERRORTYPE SoftAVC::initEncoder() {
    IV_STATUS_T status;
    WORD32 level;
 uint32_t displaySizeY;
    CHECK(!mStarted);

    OMX_ERRORTYPE errType = OMX_ErrorNone;

    displaySizeY = mWidth * mHeight;
 if (displaySizeY > (1920 * 1088)) {
        level = 50;
 } else if (displaySizeY > (1280 * 720)) {
        level = 40;
 } else if (displaySizeY > (720 * 576)) {
        level = 31;
 } else if (displaySizeY > (624 * 320)) {
        level = 30;
 } else if (displaySizeY > (352 * 288)) {
        level = 21;
 } else {
        level = 20;
 }
    mAVCEncLevel = MAX(level, mAVCEncLevel);

    mStride = mWidth;

 if (mInputDataIsMeta) {
 for (size_t i = 0; i < MAX_CONVERSION_BUFFERS; i++) {
 if (mConversionBuffers[i] != NULL) {
                free(mConversionBuffers[i]);
                mConversionBuffers[i] = 0;
 }

 if (((uint64_t)mStride * mHeight) > ((uint64_t)INT32_MAX / 3)) {
                ALOGE("Buffer size is too big.");
 return OMX_ErrorUndefined;
 }
            mConversionBuffers[i] = (uint8_t *)malloc(mStride * mHeight * 3 / 2);

 if (mConversionBuffers[i] == NULL) {
                ALOGE("Allocating conversion buffer failed.");
 return OMX_ErrorUndefined;
 }

            mConversionBuffersFree[i] = 1;
 }
 }

 switch (mColorFormat) {
 case OMX_COLOR_FormatYUV420SemiPlanar:
            mIvVideoColorFormat = IV_YUV_420SP_UV;
            ALOGV("colorFormat YUV_420SP");
 break;
 default:
 case OMX_COLOR_FormatYUV420Planar:
            mIvVideoColorFormat = IV_YUV_420P;
            ALOGV("colorFormat YUV_420P");
 break;
 }

    ALOGD("Params width %d height %d level %d colorFormat %d", mWidth,
            mHeight, mAVCEncLevel, mIvVideoColorFormat);

  
 {
 iv_num_mem_rec_ip_t s_num_mem_rec_ip;
 iv_num_mem_rec_op_t s_num_mem_rec_op;

        s_num_mem_rec_ip.u4_size = sizeof(iv_num_mem_rec_ip_t);
        s_num_mem_rec_op.u4_size = sizeof(iv_num_mem_rec_op_t);

        s_num_mem_rec_ip.e_cmd = IV_CMD_GET_NUM_MEM_REC;

        status = ive_api_function(0, &s_num_mem_rec_ip, &s_num_mem_rec_op);

 if (status != IV_SUCCESS) {
            ALOGE("Get number of memory records failed = 0x%x\n",
                    s_num_mem_rec_op.u4_error_code);
 return OMX_ErrorUndefined;
 }

        mNumMemRecords = s_num_mem_rec_op.u4_num_mem_rec;
 }

  
 if (mNumMemRecords > SIZE_MAX / sizeof(iv_mem_rec_t)) {
        ALOGE("requested memory size is too big.");
 return OMX_ErrorUndefined;
 }
    mMemRecords = (iv_mem_rec_t *)malloc(mNumMemRecords * sizeof(iv_mem_rec_t));
 if (NULL == mMemRecords) {
        ALOGE("Unable to allocate memory for hold memory records: Size %zu",
                mNumMemRecords * sizeof(iv_mem_rec_t));
        mSignalledError = true;
        notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 return OMX_ErrorUndefined;
 }

 {
 iv_mem_rec_t *ps_mem_rec;
        ps_mem_rec = mMemRecords;
 for (size_t i = 0; i < mNumMemRecords; i++) {
            ps_mem_rec->u4_size = sizeof(iv_mem_rec_t);
            ps_mem_rec->pv_base = NULL;
            ps_mem_rec->u4_mem_size = 0;
            ps_mem_rec->u4_mem_alignment = 0;
            ps_mem_rec->e_mem_type = IV_NA_MEM_TYPE;

            ps_mem_rec++;
 }
 }

  
 {
 iv_fill_mem_rec_ip_t s_fill_mem_rec_ip;
 iv_fill_mem_rec_op_t s_fill_mem_rec_op;

        s_fill_mem_rec_ip.u4_size = sizeof(iv_fill_mem_rec_ip_t);
        s_fill_mem_rec_op.u4_size = sizeof(iv_fill_mem_rec_op_t);

        s_fill_mem_rec_ip.e_cmd = IV_CMD_FILL_NUM_MEM_REC;
        s_fill_mem_rec_ip.ps_mem_rec = mMemRecords;
        s_fill_mem_rec_ip.u4_num_mem_rec = mNumMemRecords;
        s_fill_mem_rec_ip.u4_max_wd = mWidth;
        s_fill_mem_rec_ip.u4_max_ht = mHeight;
        s_fill_mem_rec_ip.u4_max_level = mAVCEncLevel;
        s_fill_mem_rec_ip.e_color_format = DEFAULT_INP_COLOR_FORMAT;
        s_fill_mem_rec_ip.u4_max_ref_cnt = DEFAULT_MAX_REF_FRM;
        s_fill_mem_rec_ip.u4_max_reorder_cnt = DEFAULT_MAX_REORDER_FRM;
        s_fill_mem_rec_ip.u4_max_srch_rng_x = DEFAULT_MAX_SRCH_RANGE_X;
        s_fill_mem_rec_ip.u4_max_srch_rng_y = DEFAULT_MAX_SRCH_RANGE_Y;

        status = ive_api_function(0, &s_fill_mem_rec_ip, &s_fill_mem_rec_op);

 if (status != IV_SUCCESS) {
            ALOGE("Fill memory records failed = 0x%x\n",
                    s_fill_mem_rec_op.u4_error_code);
            mSignalledError = true;
            notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 return OMX_ErrorUndefined;
 }
 }

  
 {
        WORD32 total_size;
 iv_mem_rec_t *ps_mem_rec;
        total_size = 0;
        ps_mem_rec = mMemRecords;

 for (size_t i = 0; i < mNumMemRecords; i++) {
            ps_mem_rec->pv_base = ive_aligned_malloc(
                    ps_mem_rec->u4_mem_alignment, ps_mem_rec->u4_mem_size);
 if (ps_mem_rec->pv_base == NULL) {
                ALOGE("Allocation failure for mem record id %zu size %u\n", i,
                        ps_mem_rec->u4_mem_size);
                mSignalledError = true;
                notify(OMX_EventError, OMX_ErrorUndefined, 0, 0);
 return OMX_ErrorUndefined;

 }
            total_size += ps_mem_rec->u4_mem_size;

            ps_mem_rec++;
 }
 }

  
 {
 ive_init_ip_t s_init_ip;
 ive_init_op_t s_init_op;

        mCodecCtx = (iv_obj_t *)mMemRecords[0].pv_base;
        mCodecCtx->u4_size = sizeof(iv_obj_t);
        mCodecCtx->pv_fxns = (void *)ive_api_function;

        s_init_ip.u4_size = sizeof(ive_init_ip_t);
        s_init_op.u4_size = sizeof(ive_init_op_t);

        s_init_ip.e_cmd = IV_CMD_INIT;
        s_init_ip.u4_num_mem_rec = mNumMemRecords;
        s_init_ip.ps_mem_rec = mMemRecords;
        s_init_ip.u4_max_wd = mWidth;
        s_init_ip.u4_max_ht = mHeight;
        s_init_ip.u4_max_ref_cnt = DEFAULT_MAX_REF_FRM;
        s_init_ip.u4_max_reorder_cnt = DEFAULT_MAX_REORDER_FRM;
        s_init_ip.u4_max_level = mAVCEncLevel;
        s_init_ip.e_inp_color_fmt = mIvVideoColorFormat;

 if (mReconEnable || mPSNREnable) {
            s_init_ip.u4_enable_recon = 1;
 } else {
            s_init_ip.u4_enable_recon = 0;
 }
        s_init_ip.e_recon_color_fmt = DEFAULT_RECON_COLOR_FORMAT;
        s_init_ip.e_rc_mode = DEFAULT_RC_MODE;
        s_init_ip.u4_max_framerate = DEFAULT_MAX_FRAMERATE;
        s_init_ip.u4_max_bitrate = DEFAULT_MAX_BITRATE;
        s_init_ip.u4_num_bframes = mBframes;
        s_init_ip.e_content_type = IV_PROGRESSIVE;
        s_init_ip.u4_max_srch_rng_x = DEFAULT_MAX_SRCH_RANGE_X;
        s_init_ip.u4_max_srch_rng_y = DEFAULT_MAX_SRCH_RANGE_Y;
        s_init_ip.e_slice_mode = mSliceMode;
        s_init_ip.u4_slice_param = mSliceParam;
        s_init_ip.e_arch = mArch;
        s_init_ip.e_soc = DEFAULT_SOC;

        status = ive_api_function(mCodecCtx, &s_init_ip, &s_init_op);

 if (status != IV_SUCCESS) {
            ALOGE("Init memory records failed = 0x%x\n",
                    s_init_op.u4_error_code);
            mSignalledError = true;
            notify(OMX_EventError, OMX_ErrorUndefined, 0  , NULL  );
 return OMX_ErrorUndefined;
 }
 }

  
    logVersion();

  
    setNumCores();

  
    setDimensions();

  
    setFrameRate();

  
    setIpeParams();

  
    setBitRate();

  
    setQp();

  
    setAirParams();

  
    setVbvParams();

  
    setMeParams();

  
    setGopParams();

  
    setDeblockParams();

  
    setProfileParams();

  
    setEncMode(IVE_ENC_MODE_HEADER);

    ALOGV("init_codec successfull");

    mSpsPpsHeaderReceived = false;
    mStarted = true;

 return OMX_ErrorNone;
}
