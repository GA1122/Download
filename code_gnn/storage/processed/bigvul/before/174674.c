WORD32 ih264d_delete_gap_frm_sliding(dpb_manager_t *ps_dpb_mgr,
                                    WORD32 i4_frame_num,
                                    UWORD8 *pu1_del_node)
{
    WORD8 i1_gap_idx, i, j, j_min;
    WORD32 *pi4_gaps_start_frm_num, *pi4_gaps_end_frm_num, i4_gap_frame_num;
    WORD32 i4_start_frm_num, i4_end_frm_num;
    WORD32 i4_max_frm_num;
    WORD32 i4_frm_num, i4_gap_frm_num_min;

  
  
 *pu1_del_node = 1;
 if(0 == ps_dpb_mgr->u1_num_gaps)
 return OK;
    pi4_gaps_start_frm_num = ps_dpb_mgr->ai4_gaps_start_frm_num;
    pi4_gaps_end_frm_num = ps_dpb_mgr->ai4_gaps_end_frm_num;
    i4_gap_frame_num = INVALID_FRAME_NUM;
    i4_max_frm_num = ps_dpb_mgr->i4_max_frm_num;

    i1_gap_idx = -1;
 if(INVALID_FRAME_NUM != i4_frame_num)
 {
        i4_gap_frame_num = i4_frame_num;
 for(i = 0; i < MAX_FRAMES; i++)
 {
            i4_start_frm_num = pi4_gaps_start_frm_num[i];
 if(INVALID_FRAME_NUM != i4_start_frm_num)
 {
                i4_end_frm_num = pi4_gaps_end_frm_num[i];
 if(i4_end_frm_num < i4_max_frm_num)
 {
 if(i4_start_frm_num <= i4_gap_frame_num)
 {
                        i4_gap_frame_num = i4_start_frm_num;
                        i1_gap_idx = i;
 }
 }
 else
 {
 if(((i4_start_frm_num <= i4_gap_frame_num)
 && (i4_gap_frame_num <= i4_max_frm_num))
 || ((i4_start_frm_num >= i4_gap_frame_num)
 && ((i4_gap_frame_num
 + i4_max_frm_num)
 >= i4_end_frm_num)))
 {
                        i4_gap_frame_num = i4_start_frm_num;
                        i1_gap_idx = i;
 }
 }
 }
 }
 }
 else
 {
  
  
        i4_gap_frame_num = pi4_gaps_start_frm_num[0];
        i1_gap_idx = 0;
 for(i = 1; i < MAX_FRAMES; i++)
 {
 if(INVALID_FRAME_NUM != pi4_gaps_start_frm_num[i])
 {
 if(pi4_gaps_start_frm_num[i] < i4_gap_frame_num)
 {
                    i4_gap_frame_num = pi4_gaps_start_frm_num[i];
                    i1_gap_idx = i;
 }
 }
 }
 if(INVALID_FRAME_NUM == i4_gap_frame_num)
 {
            UWORD32 i4_error_code;
            i4_error_code = ERROR_DBP_MANAGER_T;
 return i4_error_code;
 }
 }

 if(-1 != i1_gap_idx)
 {
  
        i4_start_frm_num = pi4_gaps_start_frm_num[i1_gap_idx];
 if(i4_start_frm_num < 0)
            i4_start_frm_num += i4_max_frm_num;
        i4_end_frm_num = pi4_gaps_end_frm_num[i1_gap_idx];
 if(i4_end_frm_num < 0)
            i4_end_frm_num += i4_max_frm_num;

        i4_gap_frm_num_min = 0xfffffff;
        j_min = MAX_FRAMES;
 for(j = 0; j < MAX_FRAMES; j++)
 {
            i4_frm_num = ps_dpb_mgr->ai4_poc_buf_id_map[j][2];
 if((i4_start_frm_num <= i4_frm_num)
 && (i4_end_frm_num >= i4_frm_num))
 {
 if(i4_frm_num < i4_gap_frm_num_min)
 {
                    j_min = j;
                    i4_gap_frm_num_min = i4_frm_num;
 }
 }
 }

 if(j_min != MAX_FRAMES)
 {

            ps_dpb_mgr->ai4_poc_buf_id_map[j_min][0] = -1;
            ps_dpb_mgr->ai4_poc_buf_id_map[j_min][1] = 0x7fffffff;
            ps_dpb_mgr->ai4_poc_buf_id_map[j_min][2] = GAP_FRAME_NUM;
            ps_dpb_mgr->i1_gaps_deleted++;

            ps_dpb_mgr->ai1_gaps_per_seq[i1_gap_idx]--;
            ps_dpb_mgr->u1_num_gaps--;
 *pu1_del_node = 0;
 if(0 == ps_dpb_mgr->ai1_gaps_per_seq[i1_gap_idx])
 {
                ps_dpb_mgr->ai4_gaps_start_frm_num[i1_gap_idx] =
                INVALID_FRAME_NUM;
                ps_dpb_mgr->ai4_gaps_end_frm_num[i1_gap_idx] = 0;
 }
 }
 }

 return OK;
}
