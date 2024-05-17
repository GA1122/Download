WORD32 ih264d_set_num_cores(iv_obj_t *dec_hdl, void *pv_api_ip, void *pv_api_op)
{
 ih264d_ctl_set_num_cores_ip_t *ps_ip;
 ih264d_ctl_set_num_cores_op_t *ps_op;
 dec_struct_t *ps_dec = dec_hdl->pv_codec_handle;

    ps_ip = (ih264d_ctl_set_num_cores_ip_t *)pv_api_ip;
    ps_op = (ih264d_ctl_set_num_cores_op_t *)pv_api_op;
    ps_op->u4_error_code = 0;
    ps_dec->u4_num_cores = ps_ip->u4_num_cores;
 if(ps_dec->u4_num_cores == 1)
 {
        ps_dec->u1_separate_parse = 0;
 }
 else
 {
        ps_dec->u1_separate_parse = 1;
 }

  
 if(ps_dec->u4_num_cores > 3)
        ps_dec->u4_num_cores = 3;

 return IV_SUCCESS;
}
