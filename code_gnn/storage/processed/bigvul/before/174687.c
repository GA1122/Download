void ih264d_release_pics_in_dpb(void *pv_dec,
                                UWORD8 u1_disp_bufs)
{
    WORD8 i;
 dec_struct_t *ps_dec = (dec_struct_t *)pv_dec;

 for(i = 0; i < u1_disp_bufs; i++)
 {
        ih264_buf_mgr_release((buf_mgr_t *)ps_dec->pv_pic_buf_mgr,
                              i,
                              BUF_MGR_REF);
        ih264_buf_mgr_release((buf_mgr_t *)ps_dec->pv_mv_buf_mgr,
                              ps_dec->au1_pic_buf_id_mv_buf_id_map[i],
                              BUF_MGR_REF);
 }
}
