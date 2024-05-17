void ih264d_release_display_field(dec_struct_t *ps_dec,
 ivd_get_display_frame_op_t *pv_disp_op)
{
 if(1 == pv_disp_op->u4_error_code)
 {
 if(1 == ps_dec->u1_flushfrm)
 {
            UWORD32 i;

 if(1 == ps_dec->u4_share_disp_buf)
 {
                H264_MUTEX_LOCK(&ps_dec->process_disp_mutex);
 for(i = 0; i < (MAX_DISP_BUFS_NEW); i++)
 {
 if(1 == ps_dec->u4_disp_buf_mapping[i])
 {
                        ih264_buf_mgr_release(
 (buf_mgr_t *)ps_dec->pv_pic_buf_mgr, i,
                                        BUF_MGR_IO);
                        ps_dec->u4_disp_buf_mapping[i] = 0;
 }
 } H264_MUTEX_UNLOCK(&ps_dec->process_disp_mutex);

                memset(ps_dec->u4_disp_buf_to_be_freed, 0,
 (MAX_DISP_BUFS_NEW) * sizeof(UWORD32));
 for(i = 0; i < ps_dec->u1_pic_bufs; i++)
                    ps_dec->u4_disp_buf_mapping[i] = 1;
 }
            ps_dec->u1_flushfrm = 0;

 }
 }
 else
 {
        H264_MUTEX_LOCK(&ps_dec->process_disp_mutex);

 if(0 == ps_dec->u4_share_disp_buf)
 {
            ih264_buf_mgr_release((buf_mgr_t *)ps_dec->pv_pic_buf_mgr,
                                  pv_disp_op->u4_disp_buf_id,
                                  BUF_MGR_IO);

 }
 else
 {
            ps_dec->u4_disp_buf_mapping[pv_disp_op->u4_disp_buf_id] = 1;
 } H264_MUTEX_UNLOCK(&ps_dec->process_disp_mutex);

 }
}
