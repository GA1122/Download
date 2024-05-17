void ih264d_assign_pic_num(dec_struct_t *ps_dec)
{
 dpb_manager_t *ps_dpb_mgr;
 struct dpb_info_t *ps_next_dpb;
    WORD8 i;
    WORD32 i4_cur_frame_num, i4_max_frame_num;
    WORD32 i4_ref_frame_num;
    UWORD8 u1_fld_pic_flag = ps_dec->ps_cur_slice->u1_field_pic_flag;

    i4_max_frame_num = ps_dec->ps_cur_sps->u2_u4_max_pic_num_minus1 + 1;
    i4_cur_frame_num = ps_dec->ps_cur_pic->i4_frame_num;
    ps_dpb_mgr = ps_dec->ps_dpb_mgr;

  
    ps_next_dpb = ps_dpb_mgr->ps_dpb_st_head;
 for(i = 0; i < ps_dpb_mgr->u1_num_st_ref_bufs; i++)
 {
        WORD32 i4_pic_num;

        i4_ref_frame_num = ps_next_dpb->ps_pic_buf->i4_frame_num;
 if(i4_ref_frame_num > i4_cur_frame_num)
 {
  
            i4_pic_num = i4_ref_frame_num - i4_max_frame_num;
 }
 else
 {
  
            i4_pic_num = i4_ref_frame_num;
 }

        ps_next_dpb->ps_pic_buf->i4_pic_num = i4_pic_num;
        ps_next_dpb->i4_frame_num = i4_pic_num;
        ps_next_dpb->ps_pic_buf->u1_long_term_frm_idx = MAX_REF_BUFS + 1;
 if(u1_fld_pic_flag)
 {
  

            ps_next_dpb->s_top_field.i4_pic_num = i4_pic_num * 2
 + !(ps_dec->ps_cur_slice->u1_bottom_field_flag);
            ps_next_dpb->s_bot_field.i4_pic_num = i4_pic_num * 2
 + ps_dec->ps_cur_slice->u1_bottom_field_flag;
 }
  
        ps_next_dpb = ps_next_dpb->ps_prev_short;
 }

 if(ps_dec->ps_cur_sps->u1_gaps_in_frame_num_value_allowed_flag
 && ps_dpb_mgr->u1_num_gaps)
 {
        WORD32 i4_start_frm, i4_end_frm;
  
 for(i = 0; i < MAX_FRAMES; i++)
 {
            i4_start_frm = ps_dpb_mgr->ai4_gaps_start_frm_num[i];
 if(i4_start_frm != INVALID_FRAME_NUM)
 {
 if(i4_start_frm > i4_cur_frame_num)
 {
  
                    i4_start_frm -= i4_max_frame_num;
 }
                ps_dpb_mgr->ai4_gaps_start_frm_num[i] = i4_start_frm;
                i4_end_frm = ps_dpb_mgr->ai4_gaps_end_frm_num[i];

 if(i4_end_frm > i4_cur_frame_num)
 {
  
                    i4_end_frm -= i4_max_frame_num;
 }
                ps_dpb_mgr->ai4_gaps_end_frm_num[i] = i4_end_frm;
 }
 }
 }
}
