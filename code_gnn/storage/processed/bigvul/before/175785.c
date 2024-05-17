WORD32 ih264d_get_num_rec(void *pv_api_ip, void *pv_api_op)
{
 iv_num_mem_rec_ip_t *ps_mem_q_ip;
 iv_num_mem_rec_op_t *ps_mem_q_op;
    ps_mem_q_ip = (iv_num_mem_rec_ip_t *)pv_api_ip;
    ps_mem_q_op = (iv_num_mem_rec_op_t *)pv_api_op;
    UNUSED(ps_mem_q_ip);
    ps_mem_q_op->u4_num_mem_rec = MEM_REC_CNT;

 return IV_SUCCESS;

}
