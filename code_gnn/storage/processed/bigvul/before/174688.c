void ih264d_reset_ref_bufs(dpb_manager_t *ps_dpb_mgr)
{
    WORD32 i;
 struct dpb_info_t *ps_dpb_info = ps_dpb_mgr->as_dpb_info;

 for(i = 0; i < MAX_REF_BUFS; i++)
 {
 if(ps_dpb_info[i].u1_used_as_ref)
 {
            ps_dpb_info[i].u1_used_as_ref = UNUSED_FOR_REF;
            ps_dpb_info[i].u1_lt_idx = MAX_REF_BUFS + 1;
            ps_dpb_info[i].ps_prev_short = NULL;
            ps_dpb_info[i].ps_prev_long = NULL;
            ps_dpb_info[i].ps_pic_buf = NULL;
            ps_dpb_info[i].s_top_field.u1_reference_info = UNUSED_FOR_REF;
            ps_dpb_info[i].s_bot_field.u1_reference_info = UNUSED_FOR_REF;
            ps_dpb_info[i].s_top_field.u1_long_term_frame_idx = MAX_REF_BUFS + 1;
            ps_dpb_info[i].s_bot_field.u1_long_term_frame_idx = MAX_REF_BUFS + 1;

            ih264d_free_ref_pic_mv_bufs(ps_dpb_mgr->pv_codec_handle,
                                        ps_dpb_info[i].u1_buf_id);
 }
 }
    ps_dpb_mgr->u1_num_st_ref_bufs = ps_dpb_mgr->u1_num_lt_ref_bufs = 0;
    ps_dpb_mgr->ps_dpb_st_head = NULL;
    ps_dpb_mgr->ps_dpb_ht_head = NULL;

  
    ps_dpb_mgr->u1_num_gaps = 0;
 for(i = 0; i < MAX_FRAMES; i++)
 {
        ps_dpb_mgr->ai4_gaps_start_frm_num[i] = INVALID_FRAME_NUM;
        ps_dpb_mgr->ai4_gaps_end_frm_num[i] = 0;
        ps_dpb_mgr->ai1_gaps_per_seq[i] = 0;
 }
}
