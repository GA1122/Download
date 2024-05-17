IV_API_CALL_STATUS_T impeg2d_api_retrieve_mem_rec(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{
    UWORD32 u4_i;
 dec_state_t *ps_dec_state;
 dec_state_multi_core_t *ps_dec_state_multi_core;
 iv_mem_rec_t *ps_mem_rec;
 iv_mem_rec_t *ps_temp_rec;



 impeg2d_retrieve_mem_rec_ip_t *ps_retr_mem_rec_ip;
 impeg2d_retrieve_mem_rec_op_t *ps_retr_mem_rec_op;

    ps_retr_mem_rec_ip  = (impeg2d_retrieve_mem_rec_ip_t *)pv_api_ip;
    ps_retr_mem_rec_op  = (impeg2d_retrieve_mem_rec_op_t *)pv_api_op;

    ps_mem_rec          = ps_retr_mem_rec_ip->s_ivd_retrieve_mem_rec_ip_t.pv_mem_rec_location;
    ps_dec_state_multi_core = (dec_state_multi_core_t *) (ps_dechdl->pv_codec_handle);
    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];
    ps_temp_rec        = ps_dec_state->pv_memTab;

 for(u4_i = 0; u4_i < (ps_dec_state->u4_num_mem_records);u4_i++)
 {
        ps_mem_rec[u4_i].u4_mem_size        = ps_temp_rec[u4_i].u4_mem_size;
        ps_mem_rec[u4_i].u4_mem_alignment   = ps_temp_rec[u4_i].u4_mem_alignment;
        ps_mem_rec[u4_i].e_mem_type         = ps_temp_rec[u4_i].e_mem_type;
        ps_mem_rec[u4_i].pv_base            = ps_temp_rec[u4_i].pv_base;
 }

    ps_retr_mem_rec_op->s_ivd_retrieve_mem_rec_op_t.u4_error_code       = IV_SUCCESS;
    ps_retr_mem_rec_op->s_ivd_retrieve_mem_rec_op_t.u4_num_mem_rec_filled   = ps_dec_state->u4_num_mem_records;

    impeg2_jobq_deinit(ps_dec_state->pv_jobq);
    IMPEG2D_PRINT_STATISTICS();


 return(IV_SUCCESS);

}
