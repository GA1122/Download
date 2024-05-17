WORD32 ih264d_rel_display_frame(iv_obj_t *dec_hdl,
 void *pv_api_ip,
 void *pv_api_op)
{

 ivd_rel_display_frame_ip_t *ps_rel_ip;
 ivd_rel_display_frame_op_t *ps_rel_op;
    UWORD32 buf_released = 0;

    UWORD32 u4_ts = -1;
 dec_struct_t *ps_dec = dec_hdl->pv_codec_handle;

    ps_rel_ip = (ivd_rel_display_frame_ip_t *)pv_api_ip;
    ps_rel_op = (ivd_rel_display_frame_op_t *)pv_api_op;
    ps_rel_op->u4_error_code = 0;
    u4_ts = ps_rel_ip->u4_disp_buf_id;

 if(0 == ps_dec->u4_share_disp_buf)
 {
        ps_dec->u4_disp_buf_mapping[u4_ts] = 0;
        ps_dec->u4_disp_buf_to_be_freed[u4_ts] = 0;
 return IV_SUCCESS;
 }

 if(ps_dec->pv_pic_buf_mgr != NULL)
 {
 if(1 == ps_dec->u4_disp_buf_mapping[u4_ts])
 {
            ih264_buf_mgr_release((buf_mgr_t *)ps_dec->pv_pic_buf_mgr,
                                  ps_rel_ip->u4_disp_buf_id,
                                  BUF_MGR_IO);
            ps_dec->u4_disp_buf_mapping[u4_ts] = 0;
            buf_released = 1;
 }
 }

 if((1 == ps_dec->u4_share_disp_buf) && (0 == buf_released))
        ps_dec->u4_disp_buf_to_be_freed[u4_ts] = 1;

 return IV_SUCCESS;
}
