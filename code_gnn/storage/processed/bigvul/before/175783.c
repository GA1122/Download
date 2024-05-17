WORD32 ih264d_get_buf_info(iv_obj_t *dec_hdl, void *pv_api_ip, void *pv_api_op)
{

 dec_struct_t * ps_dec;
    UWORD8 i = 0;  
    UWORD16 pic_wd, pic_ht;
 ivd_ctl_getbufinfo_op_t *ps_ctl_op =
 (ivd_ctl_getbufinfo_op_t*)pv_api_op;
    UNUSED(pv_api_ip);
    ps_ctl_op->u4_error_code = 0;

    ps_dec = (dec_struct_t *)(dec_hdl->pv_codec_handle);

    ps_ctl_op->u4_min_num_in_bufs = MIN_IN_BUFS;
 if(ps_dec->u1_chroma_format == IV_YUV_420P)
        ps_ctl_op->u4_min_num_out_bufs = MIN_OUT_BUFS_420;
 else if(ps_dec->u1_chroma_format == IV_YUV_422ILE)
        ps_ctl_op->u4_min_num_out_bufs = MIN_OUT_BUFS_422ILE;
 else if(ps_dec->u1_chroma_format == IV_RGB_565)
        ps_ctl_op->u4_min_num_out_bufs = MIN_OUT_BUFS_RGB565;
 else if((ps_dec->u1_chroma_format == IV_YUV_420SP_UV)
 || (ps_dec->u1_chroma_format == IV_YUV_420SP_VU))
        ps_ctl_op->u4_min_num_out_bufs = MIN_OUT_BUFS_420SP;

 else
 {
 return IV_FAIL;
 }

    ps_ctl_op->u4_num_disp_bufs = 1;

 for(i = 0; i < ps_ctl_op->u4_min_num_in_bufs; i++)
 {
        ps_ctl_op->u4_min_in_buf_size[i] = MIN_IN_BUF_SIZE;
 }

    pic_wd = ps_dec->u4_width_at_init;
    pic_ht = ps_dec->u4_height_at_init;

 if((NULL != ps_dec->ps_cur_sps) && (1 == (ps_dec->ps_cur_sps->u1_is_valid)))
 {

 if(0 == ps_dec->u4_share_disp_buf)
 {
            pic_wd = ps_dec->u2_disp_width;
            pic_ht = ps_dec->u2_disp_height;

 }
 else
 {
            pic_wd = ps_dec->u2_frm_wd_y;
            pic_ht = ps_dec->u2_frm_ht_y;
 }

 }
 else
 {
 if(1 == ps_dec->u4_share_disp_buf)
 {
            pic_wd += (PAD_LEN_Y_H << 1);
            pic_ht += (PAD_LEN_Y_V << 2);

 }
 }

 if((WORD32)ps_dec->u4_app_disp_width > pic_wd)
        pic_wd = ps_dec->u4_app_disp_width;

 if(0 == ps_dec->u4_share_disp_buf)
        ps_ctl_op->u4_num_disp_bufs = 1;
 else
 {
 if((NULL != ps_dec->ps_cur_sps) && (1 == (ps_dec->ps_cur_sps->u1_is_valid)))
 {
            UWORD32 level, width_mbs, height_mbs;

            level = ps_dec->u4_level_at_init;
            width_mbs = ps_dec->u2_frm_wd_in_mbs;
            height_mbs = ps_dec->u2_frm_ht_in_mbs;

 if((ps_dec->ps_cur_sps->u1_vui_parameters_present_flag == 1)
 && (ps_dec->ps_cur_sps->s_vui.u4_num_reorder_frames
 != 64))
 {
                ps_ctl_op->u4_num_disp_bufs =
                                ps_dec->ps_cur_sps->s_vui.u4_num_reorder_frames + 2;
 }
 else
 {
  
                ps_ctl_op->u4_num_disp_bufs = ih264d_get_dpb_size_new(
                                level, width_mbs, height_mbs);
 }

            ps_ctl_op->u4_num_disp_bufs +=
                            ps_dec->ps_cur_sps->u1_num_ref_frames + 1;

 }
 else
 {
            ps_ctl_op->u4_num_disp_bufs = ih264d_get_dpb_size_new(
                            ps_dec->u4_level_at_init,
 (ps_dec->u4_width_at_init >> 4),
 (ps_dec->u4_height_at_init >> 4));

            ps_ctl_op->u4_num_disp_bufs +=
                            ps_ctl_op->u4_num_disp_bufs;

            ps_ctl_op->u4_num_disp_bufs =
                            MIN(ps_ctl_op->u4_num_disp_bufs,
 (ps_dec->u4_num_ref_frames_at_init
 + ps_dec->u4_num_reorder_frames_at_init));

 }

        ps_ctl_op->u4_num_disp_bufs = MAX(
                        ps_ctl_op->u4_num_disp_bufs, 6);
        ps_ctl_op->u4_num_disp_bufs = MIN(
                        ps_ctl_op->u4_num_disp_bufs, 32);
 }

  
 if(ps_dec->u1_chroma_format == IV_YUV_420P)
 {
        ps_ctl_op->u4_min_out_buf_size[0] = (pic_wd * pic_ht);
        ps_ctl_op->u4_min_out_buf_size[1] = (pic_wd * pic_ht)
 >> 2;
        ps_ctl_op->u4_min_out_buf_size[2] = (pic_wd * pic_ht)
 >> 2;
 }
 else if(ps_dec->u1_chroma_format == IV_YUV_422ILE)
 {
        ps_ctl_op->u4_min_out_buf_size[0] = (pic_wd * pic_ht)
 * 2;
        ps_ctl_op->u4_min_out_buf_size[1] =
                        ps_ctl_op->u4_min_out_buf_size[2] = 0;
 }
 else if(ps_dec->u1_chroma_format == IV_RGB_565)
 {
        ps_ctl_op->u4_min_out_buf_size[0] = (pic_wd * pic_ht)
 * 2;
        ps_ctl_op->u4_min_out_buf_size[1] =
                        ps_ctl_op->u4_min_out_buf_size[2] = 0;
 }
 else if((ps_dec->u1_chroma_format == IV_YUV_420SP_UV)
 || (ps_dec->u1_chroma_format == IV_YUV_420SP_VU))
 {
        ps_ctl_op->u4_min_out_buf_size[0] = (pic_wd * pic_ht);
        ps_ctl_op->u4_min_out_buf_size[1] = (pic_wd * pic_ht)
 >> 1;
        ps_ctl_op->u4_min_out_buf_size[2] = 0;
 }
    ps_dec->u4_num_disp_bufs_requested = ps_ctl_op->u4_num_disp_bufs;

 return IV_SUCCESS;
}
