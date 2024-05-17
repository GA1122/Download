void ih264d_free_ref_pic_mv_bufs(void* pv_dec, UWORD8 pic_buf_id)
{
 dec_struct_t *ps_dec = (dec_struct_t *)pv_dec;

 if((pic_buf_id == ps_dec->u1_pic_buf_id) &&
                    ps_dec->ps_cur_slice->u1_field_pic_flag &&
 (ps_dec->u1_top_bottom_decoded == 0))
 {
 return;
 }

    ih264_buf_mgr_release((buf_mgr_t *)ps_dec->pv_pic_buf_mgr,
                          pic_buf_id,
                          BUF_MGR_REF);
    ih264_buf_mgr_release((buf_mgr_t *)ps_dec->pv_mv_buf_mgr,
                          ps_dec->au1_pic_buf_id_mv_buf_id_map[pic_buf_id],
                          BUF_MGR_REF);
}
