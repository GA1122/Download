WORD32 ihevcd_rel_display_frame(iv_obj_t *ps_codec_obj,
 void *pv_api_ip,
 void *pv_api_op)
{

 ivd_rel_display_frame_ip_t *ps_dec_rel_disp_ip;
 ivd_rel_display_frame_op_t *ps_dec_rel_disp_op;

 codec_t *ps_codec = (codec_t *)ps_codec_obj->pv_codec_handle;

    ps_dec_rel_disp_ip = (ivd_rel_display_frame_ip_t *)pv_api_ip;
    ps_dec_rel_disp_op = (ivd_rel_display_frame_op_t *)pv_api_op;

    UNUSED(ps_dec_rel_disp_op);

 if(0 == ps_codec->i4_share_disp_buf)
 {
 return IV_SUCCESS;
 }

    ihevc_buf_mgr_release((buf_mgr_t *)ps_codec->pv_pic_buf_mgr, ps_dec_rel_disp_ip->u4_disp_buf_id, BUF_MGR_DISP);

 return IV_SUCCESS;
}
