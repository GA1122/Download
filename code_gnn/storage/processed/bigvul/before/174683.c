WORD32 ih264d_insert_lt_node(dpb_manager_t *ps_dpb_mgr,
 struct dpb_info_t *ps_mov_node,
                           UWORD32 u4_lt_idx,
                           UWORD8 u1_fld_pic_flag)
{
    UWORD8 u1_mark_top_field_long_term = 0;
    UWORD8 u1_mark_bot_field_long_term = 0;

 {
 if(u1_fld_pic_flag)
 {
  

 if((ps_mov_node->s_top_field.u1_reference_info == IS_LONG_TERM)
 && (ps_mov_node->s_bot_field.u1_reference_info
 == IS_LONG_TERM))
 {
 if(ps_mov_node->u1_lt_idx == u4_lt_idx)
                    u1_mark_bot_field_long_term = 1;
 else
 {

                    UWORD32 i4_error_code;
                    i4_error_code = ERROR_DBP_MANAGER_T;

 return i4_error_code;

 }
 }
 else if(ps_mov_node->s_top_field.u1_reference_info == IS_LONG_TERM)
 {
                u1_mark_top_field_long_term = 1;
 }

 if(!(u1_mark_top_field_long_term || u1_mark_bot_field_long_term))
 {
                UWORD32 i4_error_code;
                i4_error_code = ERROR_DBP_MANAGER_T;
 return i4_error_code;
 }
 }
 else
 {
            ps_mov_node->s_top_field.u1_reference_info = IS_LONG_TERM;
            ps_mov_node->s_bot_field.u1_reference_info = IS_LONG_TERM;
            ps_mov_node->s_top_field.u1_long_term_frame_idx = u4_lt_idx;
            ps_mov_node->s_bot_field.u1_long_term_frame_idx = u4_lt_idx;
 }

        ps_mov_node->u1_lt_idx = u4_lt_idx;  
        ps_mov_node->ps_pic_buf->u1_long_term_frm_idx = u4_lt_idx;
        ps_mov_node->u1_used_as_ref = IS_LONG_TERM;

  
  
 if(ps_dpb_mgr->u1_num_lt_ref_bufs > 0)
 {
 struct dpb_info_t *ps_next_dpb = ps_dpb_mgr->ps_dpb_ht_head;
 if(u4_lt_idx < ps_next_dpb->u1_lt_idx)
 {
                ps_mov_node->ps_prev_long = ps_next_dpb;
                ps_dpb_mgr->ps_dpb_ht_head = ps_mov_node;
 }
 else
 {
                WORD32 i;
 struct dpb_info_t *ps_nxtDPB = ps_next_dpb;
                ps_next_dpb = ps_next_dpb->ps_prev_long;
 for(i = 1; i < ps_dpb_mgr->u1_num_lt_ref_bufs; i++)
 {
 if(ps_next_dpb->u1_lt_idx > u4_lt_idx)
 break;
                    ps_nxtDPB = ps_next_dpb;
                    ps_next_dpb = ps_next_dpb->ps_prev_long;
 }

                ps_nxtDPB->ps_prev_long = ps_mov_node;
                ps_mov_node->ps_prev_long = ps_next_dpb;
 }
 }
 else
 {
            ps_dpb_mgr->ps_dpb_ht_head = ps_mov_node;
            ps_mov_node->ps_prev_long = NULL;
 }
  
        ps_mov_node->ps_pic_buf->u1_is_short = 0;

  
  
  
 if(!u1_mark_bot_field_long_term)
            ps_dpb_mgr->u1_num_lt_ref_bufs++;

 }
 return OK;
}
