IV_API_CALL_STATUS_T impeg2d_api_set_num_cores(iv_obj_t *ps_dechdl,
 void *pv_api_ip,
 void *pv_api_op)
{
 impeg2d_ctl_set_num_cores_ip_t *ps_ip;
 impeg2d_ctl_set_num_cores_op_t *ps_op;
 dec_state_t *ps_dec_state;
 dec_state_multi_core_t *ps_dec_state_multi_core;

    ps_ip  = (impeg2d_ctl_set_num_cores_ip_t *)pv_api_ip;
    ps_op = (impeg2d_ctl_set_num_cores_op_t *)pv_api_op;

    ps_dec_state_multi_core = (dec_state_multi_core_t *) (ps_dechdl->pv_codec_handle);
    ps_dec_state = ps_dec_state_multi_core->ps_dec_state[0];

 if(ps_ip->u4_num_cores > 0)
 {


        WORD32 i;
 for(i = 0; i < MAX_THREADS; i++)
            ps_dec_state_multi_core->ps_dec_state[i]->i4_num_cores = ps_ip->u4_num_cores;
 }
 else
 {
        ps_dec_state->i4_num_cores = 1;
 }
    ps_op->u4_error_code = IV_SUCCESS;

 return IV_SUCCESS;
}
