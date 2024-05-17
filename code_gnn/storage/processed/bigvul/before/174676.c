void ih264d_delete_nonref_nondisplay_pics(dpb_manager_t *ps_dpb_mgr)
{
    WORD8 i;
    WORD32 (*i4_poc_buf_id_map)[3] = ps_dpb_mgr->ai4_poc_buf_id_map;

  
 for(i = 0; (i < MAX_FRAMES) && ps_dpb_mgr->i1_gaps_deleted; i++)
 {
 if(GAP_FRAME_NUM == i4_poc_buf_id_map[i][2])
 {
            ps_dpb_mgr->i1_gaps_deleted--;
            ps_dpb_mgr->i1_poc_buf_id_entries--;
            i4_poc_buf_id_map[i][0] = -1;
            i4_poc_buf_id_map[i][1] = 0x7fffffff;
            i4_poc_buf_id_map[i][2] = 0;
 }
 }
}
