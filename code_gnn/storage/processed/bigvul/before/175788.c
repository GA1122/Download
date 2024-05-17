WORD32 ih264d_init(iv_obj_t *dec_hdl, void *pv_api_ip, void *pv_api_op)
{
 ih264d_init_ip_t *ps_init_ip;
 ih264d_init_op_t *ps_init_op;
    WORD32 init_status = IV_SUCCESS;
    ps_init_ip = (ih264d_init_ip_t *)pv_api_ip;
    ps_init_op = (ih264d_init_op_t *)pv_api_op;

    init_status = ih264d_init_video_decoder(dec_hdl, ps_init_ip, ps_init_op);

 if(IV_SUCCESS != init_status)
 {
 return init_status;
 }

 return init_status;
}
