WORD32 ih264d_decode_pic_order_cnt(UWORD8 u1_is_idr_slice,
                                   UWORD32 u2_frame_num,
 pocstruct_t *ps_prev_poc,
 pocstruct_t *ps_cur_poc,
 dec_slice_params_t *ps_cur_slice,  
 dec_pic_params_t * ps_pps,
                                   UWORD8 u1_nal_ref_idc,
                                   UWORD8 u1_bottom_field_flag,
                                   UWORD8 u1_field_pic_flag,
                                   WORD32 *pi4_poc)
{
    WORD16 i1_pic_msb;
    WORD32 i4_top_field_order_cnt = 0, i4_bottom_field_order_cnt = 0;
 dec_seq_params_t *ps_seq = ps_pps->ps_sps;
    WORD32 i4_prev_frame_num_ofst;

 switch(ps_seq->u1_pic_order_cnt_type)
 {
 case 0:
  
 if(u1_is_idr_slice)
 {
                ps_prev_poc->i4_pic_order_cnt_msb = 0;
                ps_prev_poc->i4_pic_order_cnt_lsb = 0;
 }
 if(ps_prev_poc->u1_mmco_equalto5)
 {
 if(ps_prev_poc->u1_bot_field != 1)
 {
                    ps_prev_poc->i4_pic_order_cnt_msb = 0;
                    ps_prev_poc->i4_pic_order_cnt_lsb =
                                    ps_prev_poc->i4_top_field_order_count;
 }
 else
 {
                    ps_prev_poc->i4_pic_order_cnt_msb = 0;
                    ps_prev_poc->i4_pic_order_cnt_lsb = 0;
 }
 }

 if((ps_cur_poc->i4_pic_order_cnt_lsb
 < ps_prev_poc->i4_pic_order_cnt_lsb)
 && ((ps_prev_poc->i4_pic_order_cnt_lsb
 - ps_cur_poc->i4_pic_order_cnt_lsb)
 >= (ps_seq->i4_max_pic_order_cntLsb
 >> 1)))
 {
                i1_pic_msb = ps_prev_poc->i4_pic_order_cnt_msb
 + ps_seq->i4_max_pic_order_cntLsb;
 }
 else if((ps_cur_poc->i4_pic_order_cnt_lsb
 > ps_prev_poc->i4_pic_order_cnt_lsb)
 && ((ps_cur_poc->i4_pic_order_cnt_lsb
 - ps_prev_poc->i4_pic_order_cnt_lsb)
 >= (ps_seq->i4_max_pic_order_cntLsb
 >> 1)))
 {
                i1_pic_msb = ps_prev_poc->i4_pic_order_cnt_msb
 - ps_seq->i4_max_pic_order_cntLsb;
 }
 else
 {
                i1_pic_msb = ps_prev_poc->i4_pic_order_cnt_msb;
 }

 if(!u1_field_pic_flag || !u1_bottom_field_flag)
                i4_top_field_order_cnt = i1_pic_msb
 + ps_cur_poc->i4_pic_order_cnt_lsb;

 if(!u1_field_pic_flag)
 {
                i4_bottom_field_order_cnt = i4_top_field_order_cnt
 + ps_cur_poc->i4_delta_pic_order_cnt_bottom;
 }
 else if(u1_bottom_field_flag)
 {
                i4_bottom_field_order_cnt = i1_pic_msb
 + ps_cur_poc->i4_pic_order_cnt_lsb;
 }
            ps_cur_poc->i4_pic_order_cnt_msb = i1_pic_msb;
 break;

 case 1:
 {
  
            UWORD8 i;
            WORD32 prev_frame_num;
            WORD32 frame_num_ofst;
            WORD32 abs_frm_num;
            WORD32 poc_cycle_cnt, frame_num_in_poc_cycle;
            WORD32 expected_delta_poc_cycle;
            WORD32 expected_poc;

            prev_frame_num = (WORD32)ps_cur_slice->u2_frame_num;
 if(!u1_is_idr_slice)
 {
 if(ps_cur_slice->u1_mmco_equalto5)
 {
                    prev_frame_num = 0;
                    i4_prev_frame_num_ofst = 0;
 }
 else
 {
                    i4_prev_frame_num_ofst = ps_prev_poc->i4_prev_frame_num_ofst;
 }
 }
 else
                i4_prev_frame_num_ofst = 0;

  
 if(u1_is_idr_slice)
 {
                frame_num_ofst = 0;
                ps_cur_poc->i4_delta_pic_order_cnt[0] = 0;
                ps_cur_poc->i4_delta_pic_order_cnt[1] = 0;
 }
 else if(prev_frame_num > ((WORD32)u2_frame_num))
 {
                frame_num_ofst = i4_prev_frame_num_ofst
 + ps_seq->u2_u4_max_pic_num_minus1 + 1;
 }
 else
                frame_num_ofst = i4_prev_frame_num_ofst;

  
 if(0 != ps_seq->u1_num_ref_frames_in_pic_order_cnt_cycle)
                abs_frm_num = frame_num_ofst + u2_frame_num;
 else
                abs_frm_num = 0;
 if((u1_nal_ref_idc == 0) && (abs_frm_num > 0))
                abs_frm_num = abs_frm_num - 1;

  
            expected_delta_poc_cycle = 0;
 for(i = 0; i < ps_seq->u1_num_ref_frames_in_pic_order_cnt_cycle;
                            i++)
 {
                expected_delta_poc_cycle +=
                                ps_seq->i4_ofst_for_ref_frame[i];
 }

  
  
 if(abs_frm_num > 0)
 {
                poc_cycle_cnt =
                                DIV((abs_frm_num - 1),
                                    ps_seq->u1_num_ref_frames_in_pic_order_cnt_cycle);
                frame_num_in_poc_cycle =
                                MOD((abs_frm_num - 1),
                                    ps_seq->u1_num_ref_frames_in_pic_order_cnt_cycle);

                expected_poc = poc_cycle_cnt
 * expected_delta_poc_cycle;
 for(i = 0; i <= frame_num_in_poc_cycle; i++)
 {
                    expected_poc = expected_poc
 + ps_seq->i4_ofst_for_ref_frame[i];
 }
 }
 else
                expected_poc = 0;

 if(u1_nal_ref_idc == 0)
 {
                expected_poc = expected_poc
 + ps_seq->i4_ofst_for_non_ref_pic;
 }

  
 if(!u1_field_pic_flag)
 {
                i4_top_field_order_cnt = expected_poc
 + ps_cur_poc->i4_delta_pic_order_cnt[0];
                i4_bottom_field_order_cnt = i4_top_field_order_cnt
 + ps_seq->i4_ofst_for_top_to_bottom_field
 + ps_cur_poc->i4_delta_pic_order_cnt[1];
 }
 else if(!u1_bottom_field_flag)
 {
                i4_top_field_order_cnt = expected_poc
 + ps_cur_poc->i4_delta_pic_order_cnt[0];
 }
 else
 {
                i4_bottom_field_order_cnt = expected_poc
 + ps_seq->i4_ofst_for_top_to_bottom_field
 + ps_cur_poc->i4_delta_pic_order_cnt[0];
 }
  
            ps_cur_poc->i4_prev_frame_num_ofst = frame_num_ofst;
 }

 break;
 case 2:
 {
  
            WORD32 prev_frame_num;
            WORD32 frame_num_ofst;
            WORD32 tmp_poc;

            prev_frame_num = (WORD32)ps_cur_slice->u2_frame_num;
 if(!u1_is_idr_slice)
 {
 if(ps_cur_slice->u1_mmco_equalto5)
 {
                    prev_frame_num = 0;
                    i4_prev_frame_num_ofst = 0;
 }
 else
                    i4_prev_frame_num_ofst = ps_prev_poc->i4_prev_frame_num_ofst;
 }
 else
                i4_prev_frame_num_ofst = 0;

  
 if(u1_is_idr_slice)
 {
                frame_num_ofst = 0;
                ps_cur_poc->i4_delta_pic_order_cnt[0] = 0;
                ps_cur_poc->i4_delta_pic_order_cnt[1] = 0;
 }
 else if(prev_frame_num > ((WORD32)u2_frame_num))
 {
                frame_num_ofst = i4_prev_frame_num_ofst
 + ps_seq->u2_u4_max_pic_num_minus1 + 1;
 }
 else
                frame_num_ofst = i4_prev_frame_num_ofst;

  
 if(u1_is_idr_slice)
                tmp_poc = 0;
 else if(u1_nal_ref_idc == 0)
                tmp_poc = ((frame_num_ofst + u2_frame_num) << 1)
 - 1;
 else
                tmp_poc = ((frame_num_ofst + u2_frame_num) << 1);

  
 if(!u1_field_pic_flag)
 {
                i4_top_field_order_cnt = tmp_poc;
                i4_bottom_field_order_cnt = tmp_poc;
 }
 else if(!u1_bottom_field_flag)
                i4_top_field_order_cnt = tmp_poc;
 else
                i4_bottom_field_order_cnt = tmp_poc;

  
            ps_prev_poc->i4_prev_frame_num_ofst = frame_num_ofst;
            ps_cur_poc->i4_prev_frame_num_ofst = frame_num_ofst;
 }
 break;
 default:
 return ERROR_INV_POC_TYPE_T;
 break;
 }

 if(!u1_field_pic_flag)  
 {
 *pi4_poc = MIN(i4_top_field_order_cnt, i4_bottom_field_order_cnt);
        ps_pps->i4_top_field_order_cnt = i4_top_field_order_cnt;
        ps_pps->i4_bottom_field_order_cnt = i4_bottom_field_order_cnt;
 }
 else if(!u1_bottom_field_flag)
 {
 *pi4_poc = i4_top_field_order_cnt;
        ps_pps->i4_top_field_order_cnt = i4_top_field_order_cnt;
 }
 else
 {
 *pi4_poc = i4_bottom_field_order_cnt;
        ps_pps->i4_bottom_field_order_cnt = i4_bottom_field_order_cnt;
 }

    ps_pps->i4_avg_poc = *pi4_poc;

 return OK;
}
