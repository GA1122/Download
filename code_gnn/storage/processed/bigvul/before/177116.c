OMX_ERRORTYPE SoftAVCEncoder::initEncParams() {
    CHECK(mHandle != NULL);
    memset(mHandle, 0, sizeof(tagAVCHandle));
    mHandle->AVCObject = NULL;
    mHandle->userData = this;
    mHandle->CBAVC_DPBAlloc = DpbAllocWrapper;
    mHandle->CBAVC_FrameBind = BindFrameWrapper;
    mHandle->CBAVC_FrameUnbind = UnbindFrameWrapper;
    mHandle->CBAVC_Malloc = MallocWrapper;
    mHandle->CBAVC_Free = FreeWrapper;

    CHECK(mEncParams != NULL);
    memset(mEncParams, 0, sizeof(*mEncParams));
    mEncParams->rate_control = AVC_ON;
    mEncParams->initQP = 0;
    mEncParams->init_CBP_removal_delay = 1600;

    mEncParams->intramb_refresh = 0;
    mEncParams->auto_scd = AVC_ON;
    mEncParams->out_of_band_param_set = AVC_ON;
    mEncParams->poc_type = 2;
    mEncParams->log2_max_poc_lsb_minus_4 = 12;
    mEncParams->delta_poc_zero_flag = 0;
    mEncParams->offset_poc_non_ref = 0;
    mEncParams->offset_top_bottom = 0;
    mEncParams->num_ref_in_cycle = 0;
    mEncParams->offset_poc_ref = NULL;

    mEncParams->num_ref_frame = 1;
    mEncParams->num_slice_group = 1;
    mEncParams->fmo_type = 0;

    mEncParams->db_filter = AVC_ON;
    mEncParams->disable_db_idc = 0;

    mEncParams->alpha_offset = 0;
    mEncParams->beta_offset = 0;
    mEncParams->constrained_intra_pred = AVC_OFF;

    mEncParams->data_par = AVC_OFF;
    mEncParams->fullsearch = AVC_OFF;
    mEncParams->search_range = 16;
    mEncParams->sub_pel = AVC_OFF;
    mEncParams->submb_pred = AVC_OFF;
    mEncParams->rdopt_mode = AVC_OFF;
    mEncParams->bidir_pred = AVC_OFF;

    mEncParams->use_overrun_buffer = AVC_OFF;

 if (mColorFormat != OMX_COLOR_FormatYUV420Planar || mInputDataIsMeta) {
        free(mInputFrameData);
 if (((uint64_t)mWidth * mHeight) > ((uint64_t)INT32_MAX / 3)) {
            ALOGE("Buffer size is too big.");
 return OMX_ErrorUndefined;
 }
        mInputFrameData =
 (uint8_t *) malloc((mWidth * mHeight * 3 ) >> 1);
        CHECK(mInputFrameData != NULL);
 }

 if (mWidth % 16 != 0 || mHeight % 16 != 0) {
        ALOGE("Video frame size %dx%d must be a multiple of 16",
            mWidth, mHeight);
 return OMX_ErrorBadParameter;
 }

    mEncParams->width = mWidth;
    mEncParams->height = mHeight;
    mEncParams->bitrate = mBitrate;
    mEncParams->frame_rate = (1000 * mFramerate) >> 16;  
    mEncParams->CPB_size = (uint32_t) (mBitrate >> 1);

 int32_t nMacroBlocks = divUp(mWidth, 16) * divUp(mHeight, 16);
    CHECK(mSliceGroup == NULL);
 if ((size_t)nMacroBlocks > SIZE_MAX / sizeof(uint32_t)) {
        ALOGE("requested memory size is too big.");
 return OMX_ErrorUndefined;
 }
    mSliceGroup = (uint32_t *) malloc(sizeof(uint32_t) * nMacroBlocks);
    CHECK(mSliceGroup != NULL);
 for (int ii = 0, idx = 0; ii < nMacroBlocks; ++ii) {
        mSliceGroup[ii] = idx++;
 if (idx >= mEncParams->num_slice_group) {
            idx = 0;
 }
 }
    mEncParams->slice_group = mSliceGroup;

 if (mIDRFrameRefreshIntervalInSec < 0) {
        mEncParams->idr_period = -1;
 } else if (mIDRFrameRefreshIntervalInSec == 0) {
        mEncParams->idr_period = 1;  
 } else {
        mEncParams->idr_period =
 (mIDRFrameRefreshIntervalInSec * mFramerate) >> 16;  
 }

    mEncParams->profile = mAVCEncProfile;
    mEncParams->level = mAVCEncLevel;

 return OMX_ErrorNone;
}
