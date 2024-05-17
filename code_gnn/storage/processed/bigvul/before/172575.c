void ih264d_err_pic_dispbuf_mgr(dec_struct_t *ps_dec)
{
 dec_slice_params_t *ps_cur_slice = ps_dec->ps_cur_slice;
 ivd_video_decode_op_t * ps_dec_output =
 (ivd_video_decode_op_t *)ps_dec->pv_dec_out;

    ih264_buf_mgr_release((buf_mgr_t *)ps_dec->pv_pic_buf_mgr,
                          ps_dec->u1_pic_buf_id,
                          BUF_MGR_REF);
    ih264_buf_mgr_release((buf_mgr_t *)ps_dec->pv_mv_buf_mgr,
                          ps_dec->au1_pic_buf_id_mv_buf_id_map[ps_dec->u1_pic_buf_id],
                          BUF_MGR_REF);
    ih264_buf_mgr_release((buf_mgr_t *)ps_dec->pv_pic_buf_mgr,
                          ps_dec->u1_pic_buf_id,
                          BUF_MGR_IO);
}
