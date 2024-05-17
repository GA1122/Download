WORD32 ih264d_set_display_frame(iv_obj_t *dec_hdl,
 void *pv_api_ip,
 void *pv_api_op)
{

 ivd_set_display_frame_ip_t *dec_disp_ip;
 ivd_set_display_frame_op_t *dec_disp_op;

    UWORD32 i, num_mvbank_req;
 dec_struct_t * ps_dec = (dec_struct_t *)(dec_hdl->pv_codec_handle);

    dec_disp_ip = (ivd_set_display_frame_ip_t *)pv_api_ip;
    dec_disp_op = (ivd_set_display_frame_op_t *)pv_api_op;
    dec_disp_op->u4_error_code = 0;
 if((NULL != ps_dec->ps_cur_sps) && (1 == (ps_dec->ps_cur_sps->u1_is_valid)))
 {
        UWORD32 level, width_mbs, height_mbs;

        level = ps_dec->u4_level_at_init;
        width_mbs = ps_dec->u2_frm_wd_in_mbs;
        height_mbs = ps_dec->u2_frm_ht_in_mbs;

 if((ps_dec->ps_cur_sps->u1_vui_parameters_present_flag == 1)
 && (ps_dec->ps_cur_sps->s_vui.u4_num_reorder_frames != 64))
 {
            num_mvbank_req = ps_dec->ps_cur_sps->s_vui.u4_num_reorder_frames + 2;
 }
 else
 {
  
            num_mvbank_req = ih264d_get_dpb_size_new(level, width_mbs,
                                                     height_mbs);
 }

        num_mvbank_req += ps_dec->ps_cur_sps->u1_num_ref_frames + 1;
 }
 else
 {
        UWORD32 num_bufs_app, num_bufs_level;
        UWORD32 num_ref_frames, num_reorder_frames, luma_width;
        UWORD32 luma_height, level;

        num_ref_frames = ps_dec->u4_num_ref_frames_at_init;
        num_reorder_frames = ps_dec->u4_num_reorder_frames_at_init;
        level = ps_dec->u4_level_at_init;
        luma_width = ps_dec->u4_width_at_init;
        luma_height = ps_dec->u4_height_at_init;

        num_bufs_app = num_ref_frames + num_reorder_frames + 1;

 if(num_bufs_app <= 1)
            num_bufs_app = 2;

        num_bufs_level = ih264d_get_dpb_size_new(level, (luma_width >> 4),
 (luma_height >> 4));

        num_bufs_level = num_bufs_level * 2 + 1;

        num_mvbank_req = MIN(num_bufs_level, num_bufs_app);

        num_mvbank_req += ps_dec->u4_num_extra_disp_bufs_at_init;

 }

    ps_dec->u4_num_disp_bufs = 0;
 if(ps_dec->u4_share_disp_buf)
 {
        UWORD32 u4_num_bufs = dec_disp_ip->num_disp_bufs;
 if(u4_num_bufs > MAX_DISP_BUFS_NEW)
            u4_num_bufs = MAX_DISP_BUFS_NEW;

        u4_num_bufs = MIN(u4_num_bufs, MAX_DISP_BUFS_NEW);
        u4_num_bufs = MIN(u4_num_bufs, num_mvbank_req);

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
