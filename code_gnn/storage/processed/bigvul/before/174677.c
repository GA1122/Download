WORD32 ih264d_delete_st_node_or_make_lt(dpb_manager_t *ps_dpb_mgr,
                                      WORD32 i4_pic_num,
                                      UWORD32 u4_lt_idx,
                                      UWORD8 u1_fld_pic_flag)
{
    WORD32 i;
 struct dpb_info_t *ps_next_dpb;
    WORD32 i4_frame_num = i4_pic_num;
 struct dpb_info_t *ps_unmark_node = NULL;
    UWORD8 u1_del_node = 0, u1_del_st = 0;
    UWORD8 u1_reference_type = UNUSED_FOR_REF;
    WORD32 ret;

 if(u1_fld_pic_flag)
 {
        i4_frame_num = i4_frame_num >> 1;

 if(u4_lt_idx == (MAX_REF_BUFS + 1))
            u1_reference_type = UNUSED_FOR_REF;
 else
            u1_reference_type = IS_LONG_TERM;
 }

    ps_next_dpb = ps_dpb_mgr->ps_dpb_st_head;
 if((WORD32)ps_next_dpb->i4_frame_num == i4_frame_num)
 {
        ps_unmark_node = ps_next_dpb;
 }
 else
 {
 for(i = 1; i < ps_dpb_mgr->u1_num_st_ref_bufs; i++)
 {
 if((WORD32)ps_next_dpb->ps_prev_short->i4_frame_num == i4_frame_num)
 break;
            ps_next_dpb = ps_next_dpb->ps_prev_short;
 }

 if(i == ps_dpb_mgr->u1_num_st_ref_bufs)
 {
 if(ps_dpb_mgr->u1_num_gaps)
 {
                ret = ih264d_delete_gap_frm_mmco(ps_dpb_mgr, i4_frame_num, &u1_del_st);
 if(ret != OK)
 return ret;
 }
 else
 {
                UWORD32 i4_error_code;
                i4_error_code = ERROR_DBP_MANAGER_T;

 return i4_error_code;
 }

 if(u1_del_st)
 {
                UWORD32 i4_error_code;
                i4_error_code = ERROR_DBP_MANAGER_T;
 return i4_error_code;
 }
 else
 {
 return 0;
 }
 }
 else
            ps_unmark_node = ps_next_dpb->ps_prev_short;
 }

 if(u1_fld_pic_flag)
 {
  
  
  
 if(ps_unmark_node->s_top_field.i4_pic_num == i4_pic_num)
 {
            ps_unmark_node->s_top_field.u1_reference_info = u1_reference_type;
            ps_unmark_node->s_top_field.u1_long_term_frame_idx = u4_lt_idx;
 {
                UWORD8 *pu1_src = ps_unmark_node->ps_pic_buf->pu1_col_zero_flag;
                WORD32 i4_size = ((ps_dpb_mgr->u2_pic_wd
 * ps_dpb_mgr->u2_pic_ht) >> 5);
  
                memset(pu1_src, 0, i4_size);
 }
 }

 else if(ps_unmark_node->s_bot_field.i4_pic_num == i4_pic_num)
 {

            ps_unmark_node->s_bot_field.u1_reference_info = u1_reference_type;
            ps_unmark_node->s_bot_field.u1_long_term_frame_idx = u4_lt_idx;
 {
                UWORD8 *pu1_src =
                                ps_unmark_node->ps_pic_buf->pu1_col_zero_flag
 + ((ps_dpb_mgr->u2_pic_wd
 * ps_dpb_mgr->u2_pic_ht)
 >> 5);
                WORD32 i4_size = ((ps_dpb_mgr->u2_pic_wd
 * ps_dpb_mgr->u2_pic_ht) >> 5);
  
                memset(pu1_src, 0, i4_size);
 }
 }
        ps_unmark_node->u1_used_as_ref =
                        ps_unmark_node->s_top_field.u1_reference_info
 | ps_unmark_node->s_bot_field.u1_reference_info;
 }
 else
 {
        ps_unmark_node->u1_used_as_ref = UNUSED_FOR_REF;
        ps_unmark_node->s_top_field.u1_reference_info = UNUSED_FOR_REF;
        ps_unmark_node->s_bot_field.u1_reference_info = UNUSED_FOR_REF;

 {
            UWORD8 *pu1_src = ps_unmark_node->ps_pic_buf->pu1_col_zero_flag;

            WORD32 i4_size = ((ps_dpb_mgr->u2_pic_wd
 * ps_dpb_mgr->u2_pic_ht) >> 4);
  
            memset(pu1_src, 0, i4_size);
 }
 }

 if(!(ps_unmark_node->u1_used_as_ref & IS_SHORT_TERM))
 {
 if(ps_unmark_node == ps_dpb_mgr->ps_dpb_st_head)
            ps_dpb_mgr->ps_dpb_st_head = ps_next_dpb->ps_prev_short;
 else
            ps_next_dpb->ps_prev_short = ps_unmark_node->ps_prev_short;  
        ps_dpb_mgr->u1_num_st_ref_bufs--;  
        u1_del_node = 1;
 }

 if(u4_lt_idx == MAX_REF_BUFS + 1)
 {
 if(u1_del_node)
 {
            ih264d_free_ref_pic_mv_bufs(ps_dpb_mgr->pv_codec_handle,
                                        ps_unmark_node->u1_buf_id);
            ps_unmark_node->ps_prev_short = NULL;
 }
 }
 else
 {
        WORD32 i4_status;
        ret = ih264d_delete_lt_node(ps_dpb_mgr, u4_lt_idx,
                              u1_fld_pic_flag, ps_unmark_node, &i4_status);
 if(ret != OK)
 return ret;
        ret = ih264d_insert_lt_node(ps_dpb_mgr, ps_unmark_node, u4_lt_idx,
                              u1_fld_pic_flag);
 if(ret != OK)
 return ret;
 }
 return OK;
}
