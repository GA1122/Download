status_t SoftHEVC::deInitDecoder() {
 size_t i;
    IV_API_CALL_STATUS_T status;

 if (mCodecCtx) {
 ivdext_delete_ip_t s_delete_ip;
 ivdext_delete_op_t s_delete_op;

        s_delete_ip.s_ivd_delete_ip_t.u4_size = sizeof(ivdext_delete_ip_t);
        s_delete_ip.s_ivd_delete_ip_t.e_cmd = IVD_CMD_DELETE;

        s_delete_op.s_ivd_delete_op_t.u4_size = sizeof(ivdext_delete_op_t);

        status = ivdec_api_function(mCodecCtx, (void *)&s_delete_ip, (void *)&s_delete_op);
 if (status != IV_SUCCESS) {
            ALOGE("Error in delete: 0x%x",
                    s_delete_op.s_ivd_delete_op_t.u4_error_code);
 return UNKNOWN_ERROR;
 }
 }


    mChangingResolution = false;

 return OK;
}
