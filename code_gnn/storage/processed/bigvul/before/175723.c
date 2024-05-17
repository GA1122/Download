WORD32 ih264d_init_pic(dec_struct_t *ps_dec,
                       UWORD16 u2_frame_num,
                       WORD32 i4_poc,
 dec_pic_params_t *ps_pps)
{
 dec_seq_params_t *ps_seq = ps_pps->ps_sps;
 prev_seq_params_t * ps_prev_seq_params = &ps_dec->s_prev_seq_params;
    WORD32 i4_pic_bufs;
    WORD32 ret;

    ps_dec->ps_cur_slice->u2_frame_num = u2_frame_num;
    ps_dec->ps_cur_slice->i4_poc = i4_poc;
    ps_dec->ps_cur_pps = ps_pps;
    ps_dec->ps_cur_pps->pv_codec_handle = ps_dec;

    ps_dec->ps_cur_sps = ps_seq;
    ps_dec->ps_dpb_mgr->i4_max_frm_num = ps_seq->u2_u4_max_pic_num_minus1
 + 1;

    ps_dec->ps_dpb_mgr->u2_pic_ht = ps_dec->u2_pic_ht;
    ps_dec->ps_dpb_mgr->u2_pic_wd = ps_dec->u2_pic_wd;
    ps_dec->i4_pic_type = -1;
    ps_dec->i4_frametype = -1;
    ps_dec->i4_content_type = -1;

  
  
  
    ps_seq->u2_max_mb_addr =
 (ps_seq->u2_frm_wd_in_mbs
 * (ps_dec->u2_pic_ht
 >> (4
 + ps_dec->ps_cur_slice->u1_field_pic_flag)))
 - 1;
    ps_dec->u2_frm_ht_in_mbs = (ps_dec->u2_pic_ht
 >> (4 + ps_dec->ps_cur_slice->u1_field_pic_flag));

  
  
  
  
 if(!ps_dec->u1_init_dec_flag
 || ih264d_is_sps_changed(ps_prev_seq_params, ps_seq))
 {
        ps_dec->u1_max_dec_frame_buffering = ih264d_get_dpb_size(ps_seq);

        ps_dec->i4_display_delay = ps_dec->u1_max_dec_frame_buffering;
 if((1 == ps_seq->u1_vui_parameters_present_flag) &&
 (1 == ps_seq->s_vui.u1_bitstream_restriction_flag))
 {
 if(ps_seq->u1_frame_mbs_only_flag == 1)
                ps_dec->i4_display_delay = ps_seq->s_vui.u4_num_reorder_frames + 1;
 else
                ps_dec->i4_display_delay = ps_seq->s_vui.u4_num_reorder_frames * 2 + 2;
 }

 if(IVD_DECODE_FRAME_OUT == ps_dec->e_frm_out_mode)
            ps_dec->i4_display_delay = 0;

 if(ps_dec->u4_share_disp_buf == 0)
 {
 if(ps_seq->u1_frame_mbs_only_flag == 1)
                ps_dec->u1_pic_bufs = ps_dec->i4_display_delay + ps_seq->u1_num_ref_frames + 1;
 else
                ps_dec->u1_pic_bufs = ps_dec->i4_display_delay + ps_seq->u1_num_ref_frames * 2 + 2;
 }
 else
 {
            ps_dec->u1_pic_bufs = (WORD32)ps_dec->u4_num_disp_bufs;
 }

  
        ps_dec->u1_pic_bufs = MAX(ps_dec->u1_pic_bufs, 2);

 if(ps_dec->u4_share_disp_buf == 0)
            ps_dec->u1_pic_bufs = MIN(ps_dec->u1_pic_bufs,
 (H264_MAX_REF_PICS * 2));

        ps_dec->u1_max_dec_frame_buffering = MIN(
                        ps_dec->u1_max_dec_frame_buffering,
                        ps_dec->u1_pic_bufs);

  
 if(ps_dec->u1_init_dec_flag)
 {
            ih264d_release_pics_in_dpb((void *)ps_dec,
                                       ps_dec->u1_pic_bufs);
            ih264d_release_display_bufs(ps_dec);
            ih264d_reset_ref_bufs(ps_dec->ps_dpb_mgr);
 }

  
  
  
  
 if(!ps_dec->u1_init_dec_flag ||
 ((ps_seq->u1_level_idc < H264_LEVEL_3_0) ^ (ps_prev_seq_params->u1_level_idc < H264_LEVEL_3_0)))
 {
            ret = ih264d_init_dec_mb_grp(ps_dec);
 if(ret != OK)
 return ret;
 }

        ret = ih264d_allocate_dynamic_bufs(ps_dec);
 if(ret != OK)
 {
  
            ih264d_free_dynamic_bufs(ps_dec);
            ps_dec->i4_error_code = IVD_MEM_ALLOC_FAILED;
 return IVD_MEM_ALLOC_FAILED;
 }

        ret = ih264d_create_pic_buffers(ps_dec->u1_pic_bufs,
                                        ps_dec);
 if(ret != OK)
 return ret;



        ret = ih264d_create_mv_bank(ps_dec, ps_dec->u2_pic_wd,
                                    ps_dec->u2_pic_ht);
 if(ret != OK)
 return ret;

  
 if(ps_dec->u4_share_disp_buf == 1)
 {
            WORD32 i;

 for(i = 0; i < ps_dec->u1_pic_bufs; i++)
 {
                ih264_buf_mgr_set_status((buf_mgr_t *)ps_dec->pv_pic_buf_mgr, i,
                                         BUF_MGR_IO);
 }
 }

        ps_dec->u1_init_dec_flag = 1;
        ps_prev_seq_params->u2_frm_wd_in_mbs = ps_seq->u2_frm_wd_in_mbs;
        ps_prev_seq_params->u1_level_idc = ps_seq->u1_level_idc;
        ps_prev_seq_params->u1_profile_idc = ps_seq->u1_profile_idc;
        ps_prev_seq_params->u2_frm_ht_in_mbs = ps_seq->u2_frm_ht_in_mbs;
        ps_prev_seq_params->u1_frame_mbs_only_flag =
                        ps_seq->u1_frame_mbs_only_flag;
        ps_prev_seq_params->u1_direct_8x8_inference_flag =
                        ps_seq->u1_direct_8x8_inference_flag;

        ps_dec->i4_cur_display_seq = 0;
        ps_dec->i4_prev_max_display_seq = 0;
        ps_dec->i4_max_poc = 0;

 {
  
 ctxt_inc_mb_info_t *p_DefCtxt = ps_dec->p_ctxt_inc_mb_map - 1;
            UWORD8 *pu1_temp;
            WORD8 i;
            p_DefCtxt->u1_mb_type = CAB_SKIP;

            p_DefCtxt->u1_cbp = 0x0f;
            p_DefCtxt->u1_intra_chroma_pred_mode = 0;

            p_DefCtxt->u1_yuv_dc_csbp = 0x7;

            p_DefCtxt->u1_transform8x8_ctxt = 0;

            pu1_temp = (UWORD8*)p_DefCtxt->i1_ref_idx;
 for(i = 0; i < 4; i++, pu1_temp++)
 (*pu1_temp) = 0;
            pu1_temp = (UWORD8*)p_DefCtxt->u1_mv;
 for(i = 0; i < 16; i++, pu1_temp++)
 (*pu1_temp) = 0;
            ps_dec->ps_def_ctxt_mb_info = p_DefCtxt;
 }

 }
  
    ps_dec->ps_dpb_cmds->u1_dpb_commands_read = 0;

 return OK;
}
