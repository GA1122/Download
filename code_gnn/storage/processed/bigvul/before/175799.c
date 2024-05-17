WORD32 ih264d_end_of_pic_dispbuf_mgr(dec_struct_t * ps_dec)
{
 dec_slice_params_t *ps_cur_slice = ps_dec->ps_cur_slice;
    UWORD8 u1_num_of_users = 0;
    WORD32 ret;

    H264_MUTEX_LOCK(&ps_dec->process_disp_mutex);
 if(1)
 {

 {
            ih264d_delete_nonref_nondisplay_pics(ps_dec->ps_dpb_mgr);
 if(ps_cur_slice->u1_mmco_equalto5
 || (ps_cur_slice->u1_nal_unit_type == IDR_SLICE_NAL))
 {
                ps_dec->ps_cur_pic->i4_poc = 0;
 if(ps_dec->u2_total_mbs_coded
 == (ps_dec->ps_cur_sps->u2_max_mb_addr + 1))
                    ih264d_reset_ref_bufs(ps_dec->ps_dpb_mgr);
                ih264d_release_display_bufs(ps_dec);
 }
 if(ps_dec->u4_num_reorder_frames_at_init != 0)
 {
                ret = ih264d_assign_display_seq(ps_dec);
 if(ret != OK)
 return ret;
 }
 }

 if(ps_cur_slice->u1_nal_ref_idc)
 {
  
            ih264_buf_mgr_set_status((buf_mgr_t *)ps_dec->pv_pic_buf_mgr,
                                     ps_dec->u1_pic_buf_id,
                                     BUF_MGR_REF);
  
            ih264_buf_mgr_set_status((buf_mgr_t *)ps_dec->pv_mv_buf_mgr,
                                     ps_dec->au1_pic_buf_id_mv_buf_id_map[ps_dec->u1_pic_buf_id],
                                     BUF_MGR_REF);
            ps_dec->au1_pic_buf_ref_flag[ps_dec->u1_pic_buf_id] = 1;
 }

  
  
  
 if(((0 == ps_dec->u1_last_pic_not_decoded)
 && (0
 == (ps_dec->ps_cur_pic->u4_pack_slc_typ
 & ps_dec->u4_skip_frm_mask)))
 || (ps_cur_slice->u1_nal_unit_type == IDR_SLICE_NAL))
 {
  
            ih264_buf_mgr_set_status((buf_mgr_t *)ps_dec->pv_pic_buf_mgr,
                                     ps_dec->u1_pic_buf_id,
                                     BUF_MGR_IO);

 }

 if(!ps_cur_slice->u1_field_pic_flag
 || ((TOP_FIELD_ONLY | BOT_FIELD_ONLY)
 != ps_dec->u1_top_bottom_decoded))
 {
 pic_buffer_t *ps_cur_pic = ps_dec->ps_cur_pic;
            ps_cur_pic->u2_disp_width = ps_dec->u2_disp_width;
            ps_cur_pic->u2_disp_height = ps_dec->u2_disp_height >> 1;

            ps_cur_pic->u2_crop_offset_y = ps_dec->u2_crop_offset_y;
            ps_cur_pic->u2_crop_offset_uv = ps_dec->u2_crop_offset_uv;
            ps_cur_pic->u1_pic_type = 0;

            ret = ih264d_insert_pic_in_display_list(
                            ps_dec->ps_dpb_mgr,
                            ps_dec->u1_pic_buf_id,
                            ps_dec->i4_prev_max_display_seq
 + ps_dec->ps_cur_pic->i4_poc,
                            ps_dec->ps_cur_pic->i4_frame_num);
 if(ret != OK)
 return ret;

 {
 ivd_video_decode_op_t * ps_dec_output =
 (ivd_video_decode_op_t *)ps_dec->pv_dec_out;

                ps_dec_output->u4_frame_decoded_flag = 1;
 }
 if(ps_dec->au1_pic_buf_ref_flag[ps_dec->u1_pic_buf_id] == 0)
 {
                ih264_buf_mgr_release((buf_mgr_t *)ps_dec->pv_mv_buf_mgr,
                                      ps_dec->au1_pic_buf_id_mv_buf_id_map[ps_dec->u1_pic_buf_id],
                                      BUF_MGR_REF);
                ps_dec->au1_pic_buf_ref_flag[ps_dec->u1_pic_buf_id] = 0;

 }
 }
 else
 {
            H264_DEC_DEBUG_PRINT("pic not inserted display %d %d\n",
                                 ps_cur_slice->u1_field_pic_flag,
                                 ps_dec->u1_second_field);
 }

 if(!ps_cur_slice->u1_field_pic_flag
 || ((TOP_FIELD_ONLY | BOT_FIELD_ONLY)
 == ps_dec->u1_top_bottom_decoded))
 {
 if(ps_dec->u4_num_reorder_frames_at_init == 0)
 {
                ret = ih264d_assign_display_seq(ps_dec);
 if(ret != OK)
 return ret;
 }
 }
 }

    H264_MUTEX_UNLOCK(&ps_dec->process_disp_mutex);

 return OK;
}
