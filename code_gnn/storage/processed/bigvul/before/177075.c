status_t SoftMPEG2::initDecoder() {
    IV_API_CALL_STATUS_T status;

    UWORD32 u4_num_reorder_frames;
    UWORD32 u4_num_ref_frames;
    UWORD32 u4_share_disp_buf;

    mNumCores = GetCPUCoreCount();
    mWaitForI = true;

  
    u4_num_reorder_frames = 16;
    u4_num_ref_frames = 16;
    u4_share_disp_buf = 0;

 uint32_t displayStride = outputBufferWidth();
 uint32_t displayHeight = outputBufferHeight();
 uint32_t displaySizeY = displayStride * displayHeight;

 {
 iv_num_mem_rec_ip_t s_num_mem_rec_ip;
 iv_num_mem_rec_op_t s_num_mem_rec_op;

        s_num_mem_rec_ip.u4_size = sizeof(s_num_mem_rec_ip);
        s_num_mem_rec_op.u4_size = sizeof(s_num_mem_rec_op);
        s_num_mem_rec_ip.e_cmd = IV_CMD_GET_NUM_MEM_REC;

        status = ivdec_api_function(
                mCodecCtx, (void *)&s_num_mem_rec_ip, (void *)&s_num_mem_rec_op);
 if (IV_SUCCESS != status) {
            ALOGE("Error in getting mem records: 0x%x",
                    s_num_mem_rec_op.u4_error_code);
 return UNKNOWN_ERROR;
 }

        mNumMemRecords = s_num_mem_rec_op.u4_num_mem_rec;
 }

    mMemRecords = (iv_mem_rec_t *)ivd_aligned_malloc(
 128, mNumMemRecords * sizeof(iv_mem_rec_t));
 if (mMemRecords == NULL) {
        ALOGE("Allocation failure");
 return NO_MEMORY;
 }

    memset(mMemRecords, 0, mNumMemRecords * sizeof(iv_mem_rec_t));

 {
 size_t i;
 ivdext_fill_mem_rec_ip_t s_fill_mem_ip;
 ivdext_fill_mem_rec_op_t s_fill_mem_op;
 iv_mem_rec_t *ps_mem_rec;

        s_fill_mem_ip.s_ivd_fill_mem_rec_ip_t.u4_size =
 sizeof(ivdext_fill_mem_rec_ip_t);

        s_fill_mem_ip.u4_share_disp_buf = u4_share_disp_buf;
        s_fill_mem_ip.e_output_format = mIvColorFormat;
        s_fill_mem_ip.u4_deinterlace = 1;
        s_fill_mem_ip.s_ivd_fill_mem_rec_ip_t.e_cmd = IV_CMD_FILL_NUM_MEM_REC;
        s_fill_mem_ip.s_ivd_fill_mem_rec_ip_t.pv_mem_rec_location = mMemRecords;
        s_fill_mem_ip.s_ivd_fill_mem_rec_ip_t.u4_max_frm_wd = displayStride;
        s_fill_mem_ip.s_ivd_fill_mem_rec_ip_t.u4_max_frm_ht = displayHeight;
        s_fill_mem_op.s_ivd_fill_mem_rec_op_t.u4_size =
 sizeof(ivdext_fill_mem_rec_op_t);

        ps_mem_rec = mMemRecords;
 for (i = 0; i < mNumMemRecords; i++) {
            ps_mem_rec[i].u4_size = sizeof(iv_mem_rec_t);
 }

        status = ivdec_api_function(
                mCodecCtx, (void *)&s_fill_mem_ip, (void *)&s_fill_mem_op);

 if (IV_SUCCESS != status) {
            ALOGE("Error in filling mem records: 0x%x",
                    s_fill_mem_op.s_ivd_fill_mem_rec_op_t.u4_error_code);
 return UNKNOWN_ERROR;
 }
        mNumMemRecords =
            s_fill_mem_op.s_ivd_fill_mem_rec_op_t.u4_num_mem_rec_filled;

        ps_mem_rec = mMemRecords;

 for (i = 0; i < mNumMemRecords; i++) {
            ps_mem_rec->pv_base = ivd_aligned_malloc(
                    ps_mem_rec->u4_mem_alignment, ps_mem_rec->u4_mem_size);
 if (ps_mem_rec->pv_base == NULL) {
                ALOGE("Allocation failure for memory record #%zu of size %u",
                        i, ps_mem_rec->u4_mem_size);
                status = IV_FAIL;
 return NO_MEMORY;
 }

            ps_mem_rec++;
 }
 }

  
 {
 ivdext_init_ip_t s_init_ip;
 ivdext_init_op_t s_init_op;

 void *dec_fxns = (void *)ivdec_api_function;

        s_init_ip.s_ivd_init_ip_t.u4_size = sizeof(ivdext_init_ip_t);
        s_init_ip.s_ivd_init_ip_t.e_cmd = (IVD_API_COMMAND_TYPE_T)IV_CMD_INIT;
        s_init_ip.s_ivd_init_ip_t.pv_mem_rec_location = mMemRecords;
        s_init_ip.s_ivd_init_ip_t.u4_frm_max_wd = displayStride;
        s_init_ip.s_ivd_init_ip_t.u4_frm_max_ht = displayHeight;

        s_init_ip.u4_share_disp_buf = u4_share_disp_buf;
        s_init_ip.u4_deinterlace = 1;

        s_init_op.s_ivd_init_op_t.u4_size = sizeof(s_init_op);

        s_init_ip.s_ivd_init_ip_t.u4_num_mem_rec = mNumMemRecords;
        s_init_ip.s_ivd_init_ip_t.e_output_format = mIvColorFormat;

        mCodecCtx = (iv_obj_t *)mMemRecords[0].pv_base;
        mCodecCtx->pv_fxns = dec_fxns;
        mCodecCtx->u4_size = sizeof(iv_obj_t);

        status = ivdec_api_function(mCodecCtx, (void *)&s_init_ip, (void *)&s_init_op);
 if (status != IV_SUCCESS) {
            ALOGE("Error in init: 0x%x",
                    s_init_op.s_ivd_init_op_t.u4_error_code);
 return UNKNOWN_ERROR;
 }
 }

  
    resetPlugin();

  
    setParams(displayStride);

  
    setNumCores();

  
    logVersion();

  
 uint32_t bufferSize = displaySizeY * 3 / 2;
    mFlushOutBuffer = (uint8_t *)ivd_aligned_malloc(128, bufferSize);
 if (NULL == mFlushOutBuffer) {
        ALOGE("Could not allocate flushOutputBuffer of size %u", bufferSize);
 return NO_MEMORY;
 }

    mInitNeeded = false;
    mFlushNeeded = false;
 return OK;
}
