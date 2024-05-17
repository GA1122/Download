OMX_ERRORTYPE SoftAVC::releaseEncoder() {
    IV_STATUS_T status = IV_SUCCESS;
 iv_retrieve_mem_rec_ip_t s_retrieve_mem_ip;
 iv_retrieve_mem_rec_op_t s_retrieve_mem_op;
 iv_mem_rec_t *ps_mem_rec;

 if (!mStarted) {
 return OMX_ErrorNone;
 }

    s_retrieve_mem_ip.u4_size = sizeof(iv_retrieve_mem_rec_ip_t);
    s_retrieve_mem_op.u4_size = sizeof(iv_retrieve_mem_rec_op_t);
    s_retrieve_mem_ip.e_cmd = IV_CMD_RETRIEVE_MEMREC;
    s_retrieve_mem_ip.ps_mem_rec = mMemRecords;

    status = ive_api_function(mCodecCtx, &s_retrieve_mem_ip, &s_retrieve_mem_op);

 if (status != IV_SUCCESS) {
        ALOGE("Unable to retrieve memory records = 0x%x\n",
                s_retrieve_mem_op.u4_error_code);
 return OMX_ErrorUndefined;
 }

  
    ps_mem_rec = mMemRecords;
 for (size_t i = 0; i < s_retrieve_mem_op.u4_num_mem_rec_filled; i++) {
        ive_aligned_free(ps_mem_rec->pv_base);
        ps_mem_rec++;
 }

    free(mMemRecords);

 for (size_t i = 0; i < MAX_CONVERSION_BUFFERS; i++) {
 if (mConversionBuffers[i]) {
            free(mConversionBuffers[i]);
            mConversionBuffers[i] = NULL;
 }
 }

    mStarted = false;

 return OMX_ErrorNone;
}
