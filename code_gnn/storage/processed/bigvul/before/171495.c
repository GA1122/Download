WORD32 ih264d_set_display_frame(iv_obj_t *dec_hdl,
 void *pv_api_ip,
 void *pv_api_op)
{

 ivd_set_display_frame_ip_t *dec_disp_ip;
 ivd_set_display_frame_op_t *dec_disp_op;

    UWORD32 i;
 dec_struct_t * ps_dec = (dec_struct_t *)(dec_hdl->pv_codec_handle);

    dec_disp_ip = (ivd_set_display_frame_ip_t *)pv_api_ip;
    dec_disp_op = (ivd_set_display_frame_op_t *)pv_api_op;
    dec_disp_op->u4_error_code = 0;


    ps_dec->u4_num_disp_bufs = 0;
 if(ps_dec->u4_share_disp_buf)
 {
        UWORD32 u4_num_bufs = dec_disp_ip->num_disp_bufs;

        u4_num_bufs = MIN(u4_num_bufs, MAX_DISP_BUFS_NEW);

        ps_dec->u4_num_disp_bufs = u4_num_bufs;
 for(i = 0; i < u4_num_bufs; i++)
 {
            ps_dec->disp_bufs[i].u4_num_bufs =
                            dec_disp_ip->s_disp_buffer[i].u4_num_bufs;

            ps_dec->disp_bufs[i].buf[0] =
                            dec_disp_ip->s_disp_buffer[i].pu1_bufs[0];
            ps_dec->disp_bufs[i].buf[1] =
                            dec_disp_ip->s_disp_buffer[i].pu1_bufs[1];
            ps_dec->disp_bufs[i].buf[2] =
                            dec_disp_ip->s_disp_buffer[i].pu1_bufs[2];

            ps_dec->disp_bufs[i].u4_bufsize[0] =
                            dec_disp_ip->s_disp_buffer[i].u4_min_out_buf_size[0];
            ps_dec->disp_bufs[i].u4_bufsize[1] =
                            dec_disp_ip->s_disp_buffer[i].u4_min_out_buf_size[1];
            ps_dec->disp_bufs[i].u4_bufsize[2] =
                            dec_disp_ip->s_disp_buffer[i].u4_min_out_buf_size[2];

 }
 }
 return IV_SUCCESS;

}
