WORD32 ih264d_free_node_from_dpb(dpb_manager_t *ps_dpb_mgr,
                               UWORD32 u4_cur_pic_num,
                               UWORD8 u1_numRef_frames_for_seq)
{
    WORD32 i;
    UWORD8 u1_num_gaps = ps_dpb_mgr->u1_num_gaps;
 struct dpb_info_t *ps_next_dpb;
    UWORD8 u1_del_node = 1;
    WORD32 ret;

 if((ps_dpb_mgr->u1_num_st_ref_bufs + ps_dpb_mgr->u1_num_lt_ref_bufs
 + u1_num_gaps) == u1_numRef_frames_for_seq)
 {
        UWORD8 u1_new_node_flag = 1;
 if((0 == ps_dpb_mgr->u1_num_st_ref_bufs) && (0 == u1_num_gaps))
 {
 return ERROR_DBP_MANAGER_T;
 }

        ps_next_dpb = ps_dpb_mgr->ps_dpb_st_head;

 if(ps_dpb_mgr->u1_num_st_ref_bufs > 1)
 {
 if(ps_next_dpb->i4_frame_num == (WORD32)u4_cur_pic_num)
 {
  
  
  
  
                u1_new_node_flag = 0;
 }

 for(i = 1; i < (ps_dpb_mgr->u1_num_st_ref_bufs - 1); i++)
 {
 if(ps_next_dpb == NULL)
 return ERROR_DBP_MANAGER_T;

 if(ps_next_dpb->i4_frame_num == (WORD32)u4_cur_pic_num)
 {
  
  
  
  
                    u1_new_node_flag = 0;
 }
                ps_next_dpb = ps_next_dpb->ps_prev_short;
 }

 if(ps_next_dpb->ps_prev_short->ps_prev_short != NULL)
 return ERROR_DBP_MANAGER_T;

 if(u1_new_node_flag)
 {
 if(u1_num_gaps)
 {
                    ret = ih264d_delete_gap_frm_sliding(ps_dpb_mgr,
                                                        ps_next_dpb->ps_prev_short->i4_frame_num,
 &u1_del_node);
 if(ret != OK)
 return ret;
 }

 if(u1_del_node)
 {
                    ps_dpb_mgr->u1_num_st_ref_bufs--;
                    ps_next_dpb->ps_prev_short->u1_used_as_ref = UNUSED_FOR_REF;
                    ps_next_dpb->ps_prev_short->s_top_field.u1_reference_info =
                                    UNUSED_FOR_REF;
                    ps_next_dpb->ps_prev_short->s_bot_field.u1_reference_info =
                                    UNUSED_FOR_REF;
                    ih264d_free_ref_pic_mv_bufs(ps_dpb_mgr->pv_codec_handle,
                                                ps_next_dpb->ps_prev_short->u1_buf_id);
                    ps_next_dpb->ps_prev_short->ps_pic_buf = NULL;
                    ps_next_dpb->ps_prev_short = NULL;
 }
 }
 }
 else
 {
 if(ps_dpb_mgr->u1_num_st_ref_bufs)
 {
                ret = ih264d_delete_gap_frm_sliding(ps_dpb_mgr,
                                                    ps_next_dpb->i4_frame_num,
 &u1_del_node);
 if(ret != OK)
 return ret;
 if((ps_next_dpb->i4_frame_num != (WORD32)u4_cur_pic_num)
 && u1_del_node)
 {
                    ps_dpb_mgr->u1_num_st_ref_bufs--;
                    ps_next_dpb->u1_used_as_ref = FALSE;
                    ps_next_dpb->s_top_field.u1_reference_info = UNUSED_FOR_REF;
                    ps_next_dpb->s_bot_field.u1_reference_info = UNUSED_FOR_REF;
                    ih264d_free_ref_pic_mv_bufs(ps_dpb_mgr->pv_codec_handle,
                                                ps_next_dpb->u1_buf_id);
                    ps_next_dpb->ps_pic_buf = NULL;
                    ps_next_dpb = NULL;
 }
 }
 else
 {
                ret = ih264d_delete_gap_frm_sliding(ps_dpb_mgr, INVALID_FRAME_NUM, &u1_del_node);
 if(ret != OK)
 return ret;
 if(u1_del_node)
 return ERROR_DBP_MANAGER_T;
 }
 }
 }
 return OK;
}
