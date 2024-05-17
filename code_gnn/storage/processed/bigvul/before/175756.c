IV_API_CALL_STATUS_T impeg2d_api_fill_mem_rec(void *pv_api_ip,void *pv_api_op)
{

 impeg2d_fill_mem_rec_ip_t *ps_mem_q_ip;
 impeg2d_fill_mem_rec_op_t *ps_mem_q_op;


    ps_mem_q_ip = pv_api_ip;
    ps_mem_q_op = pv_api_op;


    impeg2d_fill_mem_rec((impeg2d_fill_mem_rec_ip_t *)ps_mem_q_ip,
 (impeg2d_fill_mem_rec_op_t *)ps_mem_q_op);


 return(IV_SUCCESS);

}
