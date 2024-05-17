mv_buf_t* ihevcd_mv_mgr_get_poc(buf_mgr_t *ps_mv_buf_mgr, UWORD32 abs_poc)
{
    UWORD32 i;
 mv_buf_t *ps_mv_buf = NULL;



 for(i = 0; i < ps_mv_buf_mgr->u4_max_buf_cnt; i++)
 {
        ps_mv_buf = (mv_buf_t *)ps_mv_buf_mgr->apv_ptr[i];
 if(ps_mv_buf && (ps_mv_buf->i4_abs_poc == (WORD32)abs_poc))
 {
 break;
 }
 }

 return ps_mv_buf;
}
