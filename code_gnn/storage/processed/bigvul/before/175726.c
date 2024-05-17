void ih264d_release_display_bufs(dec_struct_t *ps_dec)
{
    WORD32 i, j;
    WORD32 i4_min_poc;
    WORD32 i4_min_poc_buf_id;
    WORD32 i4_min_index;
 dpb_manager_t *ps_dpb_mgr = ps_dec->ps_dpb_mgr;
    WORD32 (*i4_poc_buf_id_map)[3] = ps_dpb_mgr->ai4_poc_buf_id_map;

    i4_min_poc = 0x7fffffff;
    i4_min_poc_buf_id = -1;
    i4_min_index = -1;

    ih264d_delete_nonref_nondisplay_pics(ps_dpb_mgr);

 for(j = 0; j < ps_dpb_mgr->i1_poc_buf_id_entries; j++)
 {
        i4_min_poc = 0x7fffffff;
 for(i = 0; i < MAX_FRAMES; i++)
 {
 if(i4_poc_buf_id_map[i][0] != -1)
 {
 if(i4_poc_buf_id_map[i][1] < i4_min_poc)
 {
                    i4_min_poc = i4_poc_buf_id_map[i][1];
                    i4_min_poc_buf_id = i4_poc_buf_id_map[i][0];
                    i4_min_index = i;
 }
 }
 }

 if(DO_NOT_DISP != i4_min_poc_buf_id)
 {
            ps_dec->i4_cur_display_seq++;
            ih264_disp_mgr_add(
 (disp_mgr_t *)ps_dec->pv_disp_buf_mgr,
                            i4_min_poc_buf_id, ps_dec->i4_cur_display_seq,
                            ps_dec->apv_buf_id_pic_buf_map[i4_min_poc_buf_id]);
            i4_poc_buf_id_map[i4_min_index][0] = -1;
            i4_poc_buf_id_map[i4_min_index][1] = 0x7fffffff;
            ps_dpb_mgr->ai4_poc_buf_id_map[i4_min_index][2] = 0;
 }
 else
 {
            i4_poc_buf_id_map[i4_min_index][0] = -1;
            i4_poc_buf_id_map[i4_min_index][1] = 0x7fffffff;
            ps_dpb_mgr->ai4_poc_buf_id_map[i4_min_index][2] = 0;
 }
 }
    ps_dpb_mgr->i1_poc_buf_id_entries = 0;
    ps_dec->i4_prev_max_display_seq = ps_dec->i4_prev_max_display_seq
 + ps_dec->i4_max_poc + ps_dec->u1_max_dec_frame_buffering
 + 1;
    ps_dec->i4_max_poc = 0;
}
