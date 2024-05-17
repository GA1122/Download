WORD32 ih264d_clr(iv_obj_t *dec_hdl, void *pv_api_ip, void *pv_api_op)
{

 dec_struct_t * ps_dec;
 iv_retrieve_mem_rec_ip_t *dec_clr_ip;
 iv_retrieve_mem_rec_op_t *dec_clr_op;

    dec_clr_ip = (iv_retrieve_mem_rec_ip_t *)pv_api_ip;
    dec_clr_op = (iv_retrieve_mem_rec_op_t *)pv_api_op;
    ps_dec = (dec_struct_t *)(dec_hdl->pv_codec_handle);

 if(ps_dec->init_done != 1)
 {
 return IV_FAIL;
 }

 if(ps_dec->pv_pic_buf_mgr)
        ih264_buf_mgr_free((buf_mgr_t *)ps_dec->pv_pic_buf_mgr);
 if(ps_dec->pv_mv_buf_mgr)
        ih264_buf_mgr_free((buf_mgr_t *)ps_dec->pv_mv_buf_mgr);

    memcpy(dec_clr_ip->pv_mem_rec_location, ps_dec->ps_mem_tab,
           MEM_REC_CNT * (sizeof(iv_mem_rec_t)));
    dec_clr_op->u4_num_mem_rec_filled = MEM_REC_CNT;

    H264_DEC_DEBUG_PRINT("The clear non-conceal num mem recs: %d\n",
                         dec_clr_op->u4_num_mem_rec_filled);

 return IV_SUCCESS;

}
