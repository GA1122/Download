WORD32 ih264d_update_default_index_list(dpb_manager_t *ps_dpb_mgr)
{
    WORD32 i;
 struct dpb_info_t *ps_next_dpb = ps_dpb_mgr->ps_dpb_st_head;

 for(i = 0; i < ps_dpb_mgr->u1_num_st_ref_bufs; i++)
 {
        ps_dpb_mgr->ps_def_dpb[i] = ps_next_dpb->ps_pic_buf;
        ps_next_dpb = ps_next_dpb->ps_prev_short;
 }

    ps_next_dpb = ps_dpb_mgr->ps_dpb_ht_head;
 for(;i< ps_dpb_mgr->u1_num_st_ref_bufs + ps_dpb_mgr->u1_num_lt_ref_bufs; i++)
 {
        ps_dpb_mgr->ps_def_dpb[i] = ps_next_dpb->ps_pic_buf;
        ps_next_dpb = ps_next_dpb->ps_prev_long;
 }
 return 0;
}
