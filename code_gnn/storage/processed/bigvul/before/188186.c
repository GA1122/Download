 WORD32 ih264d_parse_decode_slice(UWORD8 u1_is_idr_slice,
                                  UWORD8 u1_nal_ref_idc,
  dec_struct_t *ps_dec  
  )
 {
  dec_bit_stream_t * ps_bitstrm = ps_dec->ps_bitstrm;
  dec_pic_params_t *ps_pps;
  dec_seq_params_t *ps_seq;
  dec_slice_params_t *ps_cur_slice = ps_dec->ps_cur_slice;
  pocstruct_t s_tmp_poc;
     WORD32 i_delta_poc[2];
     WORD32 i4_poc = 0;
     UWORD16 u2_first_mb_in_slice, u2_frame_num;
     UWORD8 u1_field_pic_flag, u1_redundant_pic_cnt = 0, u1_slice_type;
     UWORD32 u4_idr_pic_id = 0;
     UWORD8 u1_bottom_field_flag, u1_pic_order_cnt_type;
 
     UWORD8 u1_nal_unit_type;
     UWORD32 *pu4_bitstrm_buf = ps_bitstrm->pu4_buffer;
     UWORD32 *pu4_bitstrm_ofst = &ps_bitstrm->u4_ofst;
     WORD8 i1_is_end_of_poc;
 
     WORD32 ret, end_of_frame;
     WORD32 prev_slice_err, num_mb_skipped;
     UWORD8 u1_mbaff;
  pocstruct_t *ps_cur_poc;
 
     UWORD32 u4_temp;
     WORD32 i_temp;
     UWORD32 u4_call_end_of_pic = 0;
 
   
     ps_dec->ps_dpb_cmds->u1_dpb_commands_read_slc = 0;
     u2_first_mb_in_slice = ih264d_uev(pu4_bitstrm_ofst,
                                      pu4_bitstrm_buf);
  if(u2_first_mb_in_slice
  > (ps_dec->u2_frm_ht_in_mbs * ps_dec->u2_frm_wd_in_mbs))
  {
 
  return ERROR_CORRUPTED_SLICE;
  }
 
   
  if(((u2_first_mb_in_slice << ps_cur_slice->u1_mbaff_frame_flag)
  <= ps_dec->u2_cur_mb_addr) && (ps_dec->u2_cur_mb_addr != 0)
  && (ps_dec->u4_first_slice_in_pic != 0))
  {
  return ERROR_CORRUPTED_SLICE;
  }
 
     COPYTHECONTEXT("SH: first_mb_in_slice",u2_first_mb_in_slice);
 
     u4_temp = ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
 
  if(u4_temp > 9)
  return ERROR_INV_SLC_TYPE_T;
 
     u1_slice_type = u4_temp;
     COPYTHECONTEXT("SH: slice_type",(u1_slice_type));
     ps_dec->u1_sl_typ_5_9 = 0;
   
   
   
  if(u1_slice_type > 4)
  {
         u1_slice_type -= 5;
         ps_dec->u1_sl_typ_5_9 = 1;
  }
 
  {
         UWORD32 skip;
 
  if((ps_dec->i4_app_skip_mode == IVD_SKIP_PB)
  || (ps_dec->i4_dec_skip_mode == IVD_SKIP_PB))
  {
             UWORD32 u4_bit_stream_offset = 0;
 
  if(ps_dec->u1_nal_unit_type == IDR_SLICE_NAL)
  {
                 skip = 0;
 
                 ps_dec->i4_dec_skip_mode = IVD_SKIP_NONE;
  }
  else if((I_SLICE == u1_slice_type)
  && (1 >= ps_dec->ps_cur_sps->u1_num_ref_frames))
  {
                 skip = 0;
 
                 ps_dec->i4_dec_skip_mode = IVD_SKIP_NONE;
  }
  else
  {
                 skip = 1;
  }
 
   
  if((0 == u2_first_mb_in_slice) && (1 == ps_dec->u4_prev_nal_skipped))
  {
                 skip = 0;
  }
 
  if(skip)
  {
                 ps_dec->u4_prev_nal_skipped = 1;
                 ps_dec->i4_dec_skip_mode = IVD_SKIP_PB;
  return 0;
  }
  else
  {
   
  if(1 == ps_dec->u4_prev_nal_skipped)
  {
                     ps_dec->u4_return_to_app = 1;
  return 0;
  }
  }
  }
 
  }
 
     u4_temp = ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
  if(u4_temp & MASK_ERR_PIC_SET_ID)
  return ERROR_INV_SPS_PPS_T;
   
     COPYTHECONTEXT("SH: pic_parameter_set_id", u4_temp);
     ps_pps = &ps_dec->ps_pps[u4_temp];
  if(FALSE == ps_pps->u1_is_valid)
  {
  return ERROR_INV_SPS_PPS_T;
  }
     ps_seq = ps_pps->ps_sps;
  if(!ps_seq)
  return ERROR_INV_SPS_PPS_T;
  if(FALSE == ps_seq->u1_is_valid)
  return ERROR_INV_SPS_PPS_T;
 
   
     u2_frame_num = ih264d_get_bits_h264(ps_bitstrm,
                                          ps_seq->u1_bits_in_frm_num);
 
     COPYTHECONTEXT("SH: frame_num", u2_frame_num);
 
   
  if(!ps_seq->u1_frame_mbs_only_flag)
  {
 
         u1_field_pic_flag = ih264d_get_bit_h264(ps_bitstrm);
         COPYTHECONTEXT("SH: field_pic_flag", u1_field_pic_flag);
         u1_bottom_field_flag = 0;
 
  if(u1_field_pic_flag)
  {
             ps_dec->pu1_inv_scan = (UWORD8 *)gau1_ih264d_inv_scan_fld;
             u1_bottom_field_flag = ih264d_get_bit_h264(ps_bitstrm);
             COPYTHECONTEXT("SH: bottom_field_flag", u1_bottom_field_flag);
 
  }
  else
  {
             ps_dec->pu1_inv_scan = (UWORD8 *)gau1_ih264d_inv_scan;
  }
  }
  else
  {
         u1_field_pic_flag = 0;
         u1_bottom_field_flag = 0;
 
         ps_dec->pu1_inv_scan = (UWORD8 *)gau1_ih264d_inv_scan;
  }
 
     u1_nal_unit_type = SLICE_NAL;
  if(u1_is_idr_slice)
  {
  if(0 == u1_field_pic_flag)
  {
             ps_dec->u1_top_bottom_decoded = TOP_FIELD_ONLY | BOT_FIELD_ONLY;
  }
         u1_nal_unit_type = IDR_SLICE_NAL;
         u4_idr_pic_id = ih264d_uev(pu4_bitstrm_ofst,
                                    pu4_bitstrm_buf);
  if(u4_idr_pic_id > 65535)
  return ERROR_INV_SPS_PPS_T;
         COPYTHECONTEXT("SH:  ", u4_idr_pic_id);
  }
 
   
     i_delta_poc[0] = i_delta_poc[1] = 0;
     s_tmp_poc.i4_pic_order_cnt_lsb = 0;
     s_tmp_poc.i4_delta_pic_order_cnt_bottom = 0;
     u1_pic_order_cnt_type = ps_seq->u1_pic_order_cnt_type;
  if(u1_pic_order_cnt_type == 0)
  {
         i_temp = ih264d_get_bits_h264(
                         ps_bitstrm,
                         ps_seq->u1_log2_max_pic_order_cnt_lsb_minus);
  if(i_temp < 0 || i_temp >= ps_seq->i4_max_pic_order_cntLsb)
  return ERROR_INV_SPS_PPS_T;
         s_tmp_poc.i4_pic_order_cnt_lsb = i_temp;
         COPYTHECONTEXT("SH: pic_order_cnt_lsb", s_tmp_poc.i4_pic_order_cnt_lsb);
 
  if((ps_pps->u1_pic_order_present_flag == 1) && (!u1_field_pic_flag))
  {
             s_tmp_poc.i4_delta_pic_order_cnt_bottom = ih264d_sev(
                             pu4_bitstrm_ofst, pu4_bitstrm_buf);
             COPYTHECONTEXT("SH: delta_pic_order_cnt_bottom",
                             s_tmp_poc.i4_delta_pic_order_cnt_bottom);
  }
  }
 
     s_tmp_poc.i4_delta_pic_order_cnt[0] = 0;
     s_tmp_poc.i4_delta_pic_order_cnt[1] = 0;
  if(u1_pic_order_cnt_type == 1
  && (!ps_seq->u1_delta_pic_order_always_zero_flag))
  {
         s_tmp_poc.i4_delta_pic_order_cnt[0] = ih264d_sev(pu4_bitstrm_ofst,
                                                          pu4_bitstrm_buf);
         COPYTHECONTEXT("SH: delta_pic_order_cnt[0]",
                         s_tmp_poc.i4_delta_pic_order_cnt[0]);
 
  if(ps_pps->u1_pic_order_present_flag && !u1_field_pic_flag)
  {
             s_tmp_poc.i4_delta_pic_order_cnt[1] = ih264d_sev(
                             pu4_bitstrm_ofst, pu4_bitstrm_buf);
             COPYTHECONTEXT("SH: delta_pic_order_cnt[1]",
                             s_tmp_poc.i4_delta_pic_order_cnt[1]);
  }
  }
 
  if(ps_pps->u1_redundant_pic_cnt_present_flag)
  {
         u4_temp = ih264d_uev(pu4_bitstrm_ofst, pu4_bitstrm_buf);
  if(u4_temp > MAX_REDUNDANT_PIC_CNT)
  return ERROR_INV_SPS_PPS_T;
         u1_redundant_pic_cnt = u4_temp;
         COPYTHECONTEXT("SH: redundant_pic_cnt", u1_redundant_pic_cnt);
  }
 
   
   
   
     i1_is_end_of_poc = 0;
  if(!ps_dec->u1_first_slice_in_stream)
  {
         i1_is_end_of_poc = ih264d_is_end_of_pic(u2_frame_num, u1_nal_ref_idc,
  &s_tmp_poc, &ps_dec->s_cur_pic_poc,
                                             ps_cur_slice, u1_pic_order_cnt_type,
                                             u1_nal_unit_type, u4_idr_pic_id,
                                             u1_field_pic_flag,
                                             u1_bottom_field_flag);
 
   
  if((ps_dec->u4_first_slice_in_pic == 2) && (i1_is_end_of_poc == 0))
  {
   
             ps_dec->ps_dec_err_status->u1_err_flag |= REJECT_CUR_PIC;
             i1_is_end_of_poc = 1;
  }
  else
  {
   
             ps_dec->ps_dec_err_status->u1_err_flag &= MASK_REJECT_CUR_PIC;
  }
  }
 
   
   
   
   
     u1_mbaff = ps_seq->u1_mb_aff_flag && (!u1_field_pic_flag);
     prev_slice_err = 0;
 
  if(i1_is_end_of_poc || ps_dec->u1_first_slice_in_stream)
  {
  if(u2_frame_num != ps_dec->u2_prv_frame_num
  && ps_dec->u1_top_bottom_decoded != 0
  && ps_dec->u1_top_bottom_decoded
  != (TOP_FIELD_ONLY | BOT_FIELD_ONLY))
  {
             ps_dec->u1_dangling_field = 1;
  if(ps_dec->u4_first_slice_in_pic)
  {
                 prev_slice_err = 1;
  }
  else
  {
                 prev_slice_err = 2;
  }
 
  if(ps_dec->u1_top_bottom_decoded ==TOP_FIELD_ONLY)
                 ps_cur_slice->u1_bottom_field_flag = 1;
  else
                 ps_cur_slice->u1_bottom_field_flag = 0;
 
             num_mb_skipped = (ps_dec->u2_frm_ht_in_mbs * ps_dec->u2_frm_wd_in_mbs)
  - ps_dec->u2_total_mbs_coded;
             ps_cur_poc = &ps_dec->s_cur_pic_poc;
 
             u1_is_idr_slice = ps_cur_slice->u1_nal_unit_type == IDR_SLICE_NAL;
  }
  else if(ps_dec->u4_first_slice_in_pic == 2)
  {
  if(u2_first_mb_in_slice > 0)
  {
                 prev_slice_err = 1;
                 num_mb_skipped = u2_first_mb_in_slice << u1_mbaff;
                 ps_cur_poc = &s_tmp_poc;
 
                 ps_cur_slice->u4_idr_pic_id = u4_idr_pic_id;
                 ps_cur_slice->u1_field_pic_flag = u1_field_pic_flag;
                 ps_cur_slice->u1_bottom_field_flag = u1_bottom_field_flag;
                 ps_cur_slice->i4_pic_order_cnt_lsb =
                         s_tmp_poc.i4_pic_order_cnt_lsb;
                 ps_cur_slice->u1_nal_unit_type = u1_nal_unit_type;
                 ps_cur_slice->u1_redundant_pic_cnt = u1_redundant_pic_cnt;
                 ps_cur_slice->u1_nal_ref_idc = u1_nal_ref_idc;
                 ps_cur_slice->u1_pic_order_cnt_type = u1_pic_order_cnt_type;
  }
  }
  else
  {
 
  if(ps_dec->u4_first_slice_in_pic)
  {
   
                 prev_slice_err = 1;
                 num_mb_skipped = u2_first_mb_in_slice << u1_mbaff;
  }
  else
  {
   
                 prev_slice_err = 2;
                 num_mb_skipped = (ps_dec->u2_frm_ht_in_mbs * ps_dec->u2_frm_wd_in_mbs)
  - ps_dec->u2_total_mbs_coded;
  }
             ps_cur_poc = &s_tmp_poc;
  }
  }
  else
  {
  if((u2_first_mb_in_slice << u1_mbaff) > ps_dec->u2_total_mbs_coded)
  {
             prev_slice_err = 2;
             num_mb_skipped = (u2_first_mb_in_slice << u1_mbaff)
  - ps_dec->u2_total_mbs_coded;
             ps_cur_poc = &s_tmp_poc;
  }
  else if((u2_first_mb_in_slice << u1_mbaff) < ps_dec->u2_total_mbs_coded)
  {
  return ERROR_CORRUPTED_SLICE;
  }
  }
 
  if(prev_slice_err)
  {
         ret = ih264d_mark_err_slice_skip(ps_dec, num_mb_skipped, u1_is_idr_slice, u2_frame_num, ps_cur_poc, prev_slice_err);
 
  if(ps_dec->u1_dangling_field == 1)
  {
             ps_dec->u1_second_field = 1 - ps_dec->u1_second_field;
             ps_cur_slice->u1_bottom_field_flag = u1_bottom_field_flag;
             ps_dec->u2_prv_frame_num = u2_frame_num;
             ps_dec->u1_first_slice_in_stream = 0;
  return ERROR_DANGLING_FIELD_IN_PIC;
  }
 
  if(prev_slice_err == 2)
  {
             ps_dec->u1_first_slice_in_stream = 0;
  return ERROR_INCOMPLETE_FRAME;
  }
 
  if(ps_dec->u2_total_mbs_coded
  >= ps_dec->u2_frm_ht_in_mbs * ps_dec->u2_frm_wd_in_mbs)
  {
   
             ps_dec->u1_first_slice_in_stream = 0;
  return ERROR_IN_LAST_SLICE_OF_PIC;
  }
 
  if(ps_dec->ps_dec_err_status->u1_err_flag & REJECT_CUR_PIC)
  {
             ih264d_err_pic_dispbuf_mgr(ps_dec);
  return ERROR_NEW_FRAME_EXPECTED;
  }
 
  if(ret != OK)
  return ret;
 
         i1_is_end_of_poc = 0;
  }
 
  if (ps_dec->u4_first_slice_in_pic == 0)
         ps_dec->ps_parse_cur_slice++;
 
     ps_dec->u1_slice_header_done = 0;
 
   
   
   
  if(!ps_dec->u1_first_slice_in_stream)
  {
         UWORD8 uc_mbs_exceed = 0;
 
  if(ps_dec->u2_total_mbs_coded
  == (ps_dec->ps_cur_sps->u2_max_mb_addr + 1))
  {
   
  if(ps_dec->u4_first_slice_in_pic == 0)
                 uc_mbs_exceed = 1;
  }
 
  if(i1_is_end_of_poc || uc_mbs_exceed)
  {
 
  if(1 == ps_dec->u1_last_pic_not_decoded)
  {
                 ret = ih264d_end_of_pic_dispbuf_mgr(ps_dec);
 
  if(ret != OK)
  return ret;
 
                 ret = ih264d_end_of_pic(ps_dec, u1_is_idr_slice, u2_frame_num);
  if(ret != OK)
  return ret;
 #if WIN32
                 H264_DEC_DEBUG_PRINT(" ------ PIC SKIPPED ------\n");
 #endif
  return RET_LAST_SKIP;
  }
  else
  {
                 ret = ih264d_end_of_pic(ps_dec, u1_is_idr_slice, u2_frame_num);
  if(ret != OK)
  return ret;
  }
 
  }
  }
 
  if(u1_field_pic_flag)
  {
         ps_dec->u2_prv_frame_num = u2_frame_num;
  }
 
  if(ps_cur_slice->u1_mmco_equalto5)
  {
         WORD32 i4_temp_poc;
         WORD32 i4_top_field_order_poc, i4_bot_field_order_poc;
 
  if(!ps_cur_slice->u1_field_pic_flag)  
  {
             i4_top_field_order_poc = ps_dec->ps_cur_pic->i4_top_field_order_cnt;
             i4_bot_field_order_poc =
                             ps_dec->ps_cur_pic->i4_bottom_field_order_cnt;
             i4_temp_poc = MIN(i4_top_field_order_poc,
                                      i4_bot_field_order_poc);
  }
  else if(!ps_cur_slice->u1_bottom_field_flag)
             i4_temp_poc = ps_dec->ps_cur_pic->i4_top_field_order_cnt;
  else
             i4_temp_poc = ps_dec->ps_cur_pic->i4_bottom_field_order_cnt;
 
         ps_dec->ps_cur_pic->i4_top_field_order_cnt = i4_temp_poc
  - ps_dec->ps_cur_pic->i4_top_field_order_cnt;
         ps_dec->ps_cur_pic->i4_bottom_field_order_cnt = i4_temp_poc
  - ps_dec->ps_cur_pic->i4_bottom_field_order_cnt;
         ps_dec->ps_cur_pic->i4_poc = i4_temp_poc;
         ps_dec->ps_cur_pic->i4_avg_poc = i4_temp_poc;
  }
  if(ps_dec->u4_first_slice_in_pic == 2)
  {
         ret = ih264d_decode_pic_order_cnt(u1_is_idr_slice, u2_frame_num,
  &ps_dec->s_prev_pic_poc,
  &s_tmp_poc, ps_cur_slice, ps_pps,
                                           u1_nal_ref_idc,
                                           u1_bottom_field_flag,
                                           u1_field_pic_flag, &i4_poc);
  if(ret != OK)
  return ret;
   
  if(i4_poc >= ps_dec->i4_max_poc)
             ps_dec->i4_max_poc = i4_poc;
   
  if(i4_poc == 0)
  {
             ps_dec->i4_prev_max_display_seq = ps_dec->i4_prev_max_display_seq
  + ps_dec->i4_max_poc
  + ps_dec->u1_max_dec_frame_buffering + 1;
             ps_dec->i4_max_poc = 0;
  }
  }
 
   
   
   
   
   
     ps_cur_slice->i4_delta_pic_order_cnt[0] = i_delta_poc[0];
     ps_cur_slice->i4_delta_pic_order_cnt[1] = i_delta_poc[1];
     ps_cur_slice->u4_idr_pic_id = u4_idr_pic_id;
     ps_cur_slice->u2_first_mb_in_slice = u2_first_mb_in_slice;
     ps_cur_slice->u1_field_pic_flag = u1_field_pic_flag;
     ps_cur_slice->u1_bottom_field_flag = u1_bottom_field_flag;
     ps_cur_slice->u1_slice_type = u1_slice_type;
     ps_cur_slice->i4_pic_order_cnt_lsb = s_tmp_poc.i4_pic_order_cnt_lsb;
 
     ps_cur_slice->u1_nal_unit_type = u1_nal_unit_type;
     ps_cur_slice->u1_redundant_pic_cnt = u1_redundant_pic_cnt;
     ps_cur_slice->u1_nal_ref_idc = u1_nal_ref_idc;
     ps_cur_slice->u1_pic_order_cnt_type = u1_pic_order_cnt_type;
 
  if(ps_seq->u1_frame_mbs_only_flag)
         ps_cur_slice->u1_direct_8x8_inference_flag =
                         ps_seq->u1_direct_8x8_inference_flag;
  else
         ps_cur_slice->u1_direct_8x8_inference_flag = 1;
 
  if(u1_slice_type == B_SLICE)
  {
         ps_cur_slice->u1_direct_spatial_mv_pred_flag = ih264d_get_bit_h264(
                         ps_bitstrm);
         COPYTHECONTEXT("SH: direct_spatial_mv_pred_flag",
                         ps_cur_slice->u1_direct_spatial_mv_pred_flag);
 
  if(ps_cur_slice->u1_direct_spatial_mv_pred_flag)
             ps_cur_slice->pf_decodeDirect = ih264d_decode_spatial_direct;
  else
             ps_cur_slice->pf_decodeDirect = ih264d_decode_temporal_direct;
  if(!((ps_pps->ps_sps->u1_mb_aff_flag) && (!u1_field_pic_flag)))
             ps_dec->pf_mvpred = ih264d_mvpred_nonmbaffB;
  }
  else
  {
  if(!((ps_pps->ps_sps->u1_mb_aff_flag) && (!u1_field_pic_flag)))
             ps_dec->pf_mvpred = ih264d_mvpred_nonmbaff;
  }
 
  if(ps_dec->u4_first_slice_in_pic == 2)
  {
  if(u2_first_mb_in_slice == 0)
  {
             ret = ih264d_start_of_pic(ps_dec, i4_poc, &s_tmp_poc, u2_frame_num, ps_pps);
  if(ret != OK)
  return ret;
  }
 
         ps_dec->u4_output_present = 0;
 
  {
             ih264d_get_next_display_field(ps_dec,
                                           ps_dec->ps_out_buffer,
  &(ps_dec->s_disp_op));
   
 
  if(0 != ps_dec->s_disp_op.u4_error_code)
  {
                 ps_dec->u4_fmt_conv_cur_row = ps_dec->s_disp_frame_info.u4_y_ht;
  }
  else
                 ps_dec->u4_output_present = 1;
  }
  if(ps_dec->u1_separate_parse == 1)
  {
  if(ps_dec->u4_dec_thread_created == 0)
  {
                 ithread_create(ps_dec->pv_dec_thread_handle, NULL,
  (void *)ih264d_decode_picture_thread,
  (void *)ps_dec);
 
                 ps_dec->u4_dec_thread_created = 1;
  }
 
  if((ps_dec->u4_num_cores == 3) &&
  ((ps_dec->u4_app_disable_deblk_frm == 0) || ps_dec->i1_recon_in_thread3_flag)
  && (ps_dec->u4_bs_deblk_thread_created == 0))
  {
                 ps_dec->u4_start_recon_deblk = 0;
                 ithread_create(ps_dec->pv_bs_deblk_thread_handle, NULL,
  (void *)ih264d_recon_deblk_thread,
  (void *)ps_dec);
                 ps_dec->u4_bs_deblk_thread_created = 1;
  }
  }
 
  }
 
   
  {
         UWORD8 uc_nofield_nombaff;
 
 
 
         uc_nofield_nombaff = ((ps_dec->ps_cur_slice->u1_field_pic_flag == 0)
  && (ps_dec->ps_cur_slice->u1_mbaff_frame_flag == 0)
  && (u1_slice_type != B_SLICE)
  && (ps_dec->ps_cur_pps->u1_wted_pred_flag == 0));
 
   
 
  if(uc_nofield_nombaff)
  {
             ps_dec->p_form_mb_part_info = ih264d_form_mb_part_info_bp;
             ps_dec->p_motion_compensate = ih264d_motion_compensate_bp;
  }
  else
  {
             ps_dec->p_form_mb_part_info = ih264d_form_mb_part_info_mp;
             ps_dec->p_motion_compensate = ih264d_motion_compensate_mp;
  }
 
 
  }
 
   
  {
  dec_err_status_t * ps_err = ps_dec->ps_dec_err_status;
  if(ps_err->u4_frm_sei_sync == u2_frame_num)
  {
             ps_err->u1_err_flag = ACCEPT_ALL_PICS;
             ps_err->u4_frm_sei_sync = SYNC_FRM_DEFAULT;
  }
         ps_err->u4_cur_frm = u2_frame_num;
  }
 
   
  {
         WORD32 i4_skip_b_pic, i4_skip_p_pic;
 
         i4_skip_b_pic = (ps_dec->u4_skip_frm_mask & B_SLC_BIT)
  && (B_SLICE == u1_slice_type) && (0 == u1_nal_ref_idc);
 
         i4_skip_p_pic = (ps_dec->u4_skip_frm_mask & P_SLC_BIT)
  && (P_SLICE == u1_slice_type) && (0 == u1_nal_ref_idc);
 
   
   
   
   
   
  if(i4_skip_b_pic)
  {
             ps_dec->ps_cur_pic->u4_pack_slc_typ |= B_SLC_BIT;
   
   
             ps_dec->u1_last_pic_not_decoded = 1;
 
  return OK;
  }
   
   
   
   
   
  if(i4_skip_p_pic)
  {
             ps_dec->ps_cur_pic->u4_pack_slc_typ |= P_SLC_BIT;
   
   
             ps_dec->u1_last_pic_not_decoded = 1;
 
  return OK;
  }
  }
 
  {
         UWORD16 u2_mb_x, u2_mb_y;
 
         ps_dec->i4_submb_ofst = ((u2_first_mb_in_slice
  << ps_cur_slice->u1_mbaff_frame_flag) * SUB_BLK_SIZE)
  - SUB_BLK_SIZE;
  if(u2_first_mb_in_slice)
  {
             UWORD8 u1_mb_aff;
             UWORD8 u1_field_pic;
             UWORD16 u2_frm_wd_in_mbs;
             u2_frm_wd_in_mbs = ps_seq->u2_frm_wd_in_mbs;
             u1_mb_aff = ps_cur_slice->u1_mbaff_frame_flag;
             u1_field_pic = ps_cur_slice->u1_field_pic_flag;
 
  {
                 UWORD32 x_offset;
                 UWORD32 y_offset;
                 UWORD32 u4_frame_stride;
  tfr_ctxt_t *ps_trns_addr;  
 
  if(ps_dec->u1_separate_parse)
  {
                     ps_trns_addr = &ps_dec->s_tran_addrecon_parse;
  }
  else
  {
                     ps_trns_addr = &ps_dec->s_tran_addrecon;
  }
                 u2_mb_x = MOD(u2_first_mb_in_slice, u2_frm_wd_in_mbs);
                 u2_mb_y = DIV(u2_first_mb_in_slice, u2_frm_wd_in_mbs);
 
                 u2_mb_y <<= u1_mb_aff;
 
  if((u2_mb_x > u2_frm_wd_in_mbs - 1)
  || (u2_mb_y > ps_dec->u2_frm_ht_in_mbs - 1))
  {
  return ERROR_CORRUPTED_SLICE;
  }
 
                 u4_frame_stride = ps_dec->u2_frm_wd_y << u1_field_pic;
                 x_offset = u2_mb_x << 4;
                 y_offset = (u2_mb_y * u4_frame_stride) << 4;
 
                 ps_trns_addr->pu1_dest_y = ps_dec->s_cur_pic.pu1_buf1 + x_offset
  + y_offset;
 
                 u4_frame_stride = ps_dec->u2_frm_wd_uv << u1_field_pic;
                 x_offset >>= 1;
                 y_offset = (u2_mb_y * u4_frame_stride) << 3;
 
                 x_offset *= YUV420SP_FACTOR;
 
                 ps_trns_addr->pu1_dest_u = ps_dec->s_cur_pic.pu1_buf2 + x_offset
  + y_offset;
                 ps_trns_addr->pu1_dest_v = ps_dec->s_cur_pic.pu1_buf3 + x_offset
  + y_offset;
 
                 ps_trns_addr->pu1_mb_y = ps_trns_addr->pu1_dest_y;
                 ps_trns_addr->pu1_mb_u = ps_trns_addr->pu1_dest_u;
                 ps_trns_addr->pu1_mb_v = ps_trns_addr->pu1_dest_v;
 
 
  if(ps_dec->u1_separate_parse == 1)
  {
                     ps_dec->ps_deblk_mbn = ps_dec->ps_deblk_pic
  + (u2_first_mb_in_slice << u1_mb_aff);
  }
  else
  {
                         ps_dec->ps_deblk_mbn = ps_dec->ps_deblk_pic
  + (u2_first_mb_in_slice << u1_mb_aff);
  }
 
                 ps_dec->u2_cur_mb_addr = (u2_first_mb_in_slice << u1_mb_aff);
 
                 ps_dec->ps_mv_cur = ps_dec->s_cur_pic.ps_mv
  + ((u2_first_mb_in_slice << u1_mb_aff) << 4);
  }
  }
  else
  {
  tfr_ctxt_t *ps_trns_addr;
 
  if(ps_dec->u1_separate_parse)
  {
                 ps_trns_addr = &ps_dec->s_tran_addrecon_parse;
  }
  else
  {
                 ps_trns_addr = &ps_dec->s_tran_addrecon;
  }
 
             u2_mb_x = 0xffff;
             u2_mb_y = 0;
             ps_dec->u2_cur_mb_addr = 0;
             ps_dec->ps_deblk_mbn = ps_dec->ps_deblk_pic;
             ps_dec->ps_mv_cur = ps_dec->s_cur_pic.ps_mv;
             ps_trns_addr->pu1_dest_y = ps_dec->s_cur_pic.pu1_buf1;
             ps_trns_addr->pu1_dest_u = ps_dec->s_cur_pic.pu1_buf2;
             ps_trns_addr->pu1_dest_v = ps_dec->s_cur_pic.pu1_buf3;
 
             ps_trns_addr->pu1_mb_y = ps_trns_addr->pu1_dest_y;
             ps_trns_addr->pu1_mb_u = ps_trns_addr->pu1_dest_u;
             ps_trns_addr->pu1_mb_v = ps_trns_addr->pu1_dest_v;
 
  }
 
         ps_dec->ps_part = ps_dec->ps_parse_part_params;
 
         ps_dec->u2_mbx =
  (MOD(u2_first_mb_in_slice - 1, ps_seq->u2_frm_wd_in_mbs));
         ps_dec->u2_mby =
  (DIV(u2_first_mb_in_slice - 1, ps_seq->u2_frm_wd_in_mbs));
         ps_dec->u2_mby <<= ps_cur_slice->u1_mbaff_frame_flag;
         ps_dec->i2_prev_slice_mbx = ps_dec->u2_mbx;
         ps_dec->i2_prev_slice_mby = ps_dec->u2_mby;
  }
 
   
     ps_bitstrm->u4_max_ofst += ps_dec->ps_cur_pps->u1_entropy_coding_mode;
 
     ps_dec->u1_B = (u1_slice_type == B_SLICE);
     ps_dec->u4_next_mb_skip = 0;
 
     ps_dec->ps_parse_cur_slice->u4_first_mb_in_slice =
                     ps_dec->ps_cur_slice->u2_first_mb_in_slice;
     ps_dec->ps_parse_cur_slice->slice_type =
                     ps_dec->ps_cur_slice->u1_slice_type;
 
 
     ps_dec->u4_start_recon_deblk = 1;
  {
         WORD32 num_entries;
         WORD32 size;
         UWORD8 *pu1_buf;
 
         num_entries = MAX_FRAMES;
  if((1 >= ps_dec->ps_cur_sps->u1_num_ref_frames) &&
  (0 == ps_dec->i4_display_delay))
  {
             num_entries = 1;
  }
         num_entries = ((2 * num_entries) + 1);
  if(BASE_PROFILE_IDC != ps_dec->ps_cur_sps->u1_profile_idc)
  {
             num_entries *= 2;
  }
 
         size = num_entries * sizeof(void *);
         size += PAD_MAP_IDX_POC * sizeof(void *);
 
         pu1_buf = (UWORD8 *)ps_dec->pv_map_ref_idx_to_poc_buf;
         pu1_buf += size * ps_dec->u2_cur_slice_num;
         ps_dec->ps_parse_cur_slice->ppv_map_ref_idx_to_poc = ( void *)pu1_buf;
  }
 
  if(ps_dec->u1_separate_parse)
  {
         ps_dec->ps_parse_cur_slice->pv_tu_coeff_data_start = ps_dec->pv_parse_tu_coeff_data;
  }
  else
  {
 
          ps_dec->pv_proc_tu_coeff_data = ps_dec->pv_parse_tu_coeff_data;
      }
  
    ps_dec->pu4_wt_ofsts = ps_dec->pu4_wts_ofsts_mat;
      if(u1_slice_type == I_SLICE)
      {
          ps_dec->ps_cur_pic->u4_pack_slc_typ |= I_SLC_BIT;
 
         ret = ih264d_parse_islice(ps_dec, u2_first_mb_in_slice);
 
  if(ps_dec->i4_pic_type != B_SLICE && ps_dec->i4_pic_type != P_SLICE)
             ps_dec->i4_pic_type = I_SLICE;
 
  }
  else if(u1_slice_type == P_SLICE)
  {
         ps_dec->ps_cur_pic->u4_pack_slc_typ |= P_SLC_BIT;
         ret = ih264d_parse_pslice(ps_dec, u2_first_mb_in_slice);
         ps_dec->u1_pr_sl_type = u1_slice_type;
  if(ps_dec->i4_pic_type != B_SLICE)
             ps_dec->i4_pic_type = P_SLICE;
  }
  else if(u1_slice_type == B_SLICE)
  {
         ps_dec->ps_cur_pic->u4_pack_slc_typ |= B_SLC_BIT;
         ret = ih264d_parse_bslice(ps_dec, u2_first_mb_in_slice);
         ps_dec->u1_pr_sl_type = u1_slice_type;
         ps_dec->i4_pic_type = B_SLICE;
  }
  else
  return ERROR_INV_SLC_TYPE_T;
 
  if(ps_dec->u1_slice_header_done)
  {
   
   
         ps_dec->u4_first_slice_in_pic = 0;
         ps_dec->u1_first_slice_in_stream = 0;
  }
 
  if(ret != OK)
  return ret;
 
     ps_dec->u2_cur_slice_num++;
   
     ps_dec->i2_prev_slice_mbx = ps_dec->u2_mbx;
     ps_dec->i2_prev_slice_mby = ps_dec->u2_mby;
 
   
 
  if(ps_dec->u2_total_mbs_coded >= (ps_seq->u2_max_mb_addr + 1))
  {
         ps_dec->u1_pic_decode_done = 1;
 
  }
 
  {
  dec_err_status_t * ps_err = ps_dec->ps_dec_err_status;
  if((ps_err->u1_err_flag & REJECT_PB_PICS)
  && (ps_err->u1_cur_pic_type == PIC_TYPE_I))
  {
             ps_err->u1_err_flag = ACCEPT_ALL_PICS;
  }
  }
 
     PRINT_BIN_BIT_RATIO(ps_dec)
 
  return ret;
 }