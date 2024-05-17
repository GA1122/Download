WORD32 ih264d_delete_gap_frm_mmco(dpb_manager_t *ps_dpb_mgr,
                                  WORD32 i4_frame_num,
                                  UWORD8 *pu1_del_node)
{
    WORD8 i, j;
    WORD32 *pi4_start, *pi4_end;
    WORD32 i4_start_frm_num, i4_end_frm_num, i4_max_frm_num;

  
  
 *pu1_del_node = 1;
    pi4_start = ps_dpb_mgr->ai4_gaps_start_frm_num;
    pi4_end = ps_dpb_mgr->ai4_gaps_end_frm_num;
    i4_max_frm_num = ps_dpb_mgr->i4_max_frm_num;

 if(0 == ps_dpb_mgr->u1_num_gaps)
 return OK;

 if(i4_frame_num < 0)
        i4_frame_num += i4_max_frm_num;
 for(i = 0; i < MAX_FRAMES; i++)
 {
        i4_start_frm_num = pi4_start[i];
 if(i4_start_frm_num < 0)
            i4_start_frm_num += i4_max_frm_num;
 if(INVALID_FRAME_NUM != i4_start_frm_num)
 {
            i4_end_frm_num = pi4_end[i];
 if(i4_end_frm_num < 0)
                i4_end_frm_num += i4_max_frm_num;

 if((i4_frame_num >= i4_start_frm_num)
 && (i4_frame_num <= i4_end_frm_num))
 {
 break;
 }
 else
 {
 if(((i4_frame_num + i4_max_frm_num) >= i4_start_frm_num)
 && ((i4_frame_num + i4_max_frm_num)
 <= i4_end_frm_num))
 {
                    UWORD32 i4_error_code;
                    i4_error_code = ERROR_DBP_MANAGER_T;
 return i4_error_code;
 }
 }
 }
 }

  
 for(j = 0; j < MAX_FRAMES; j++)
 {
 if(i4_frame_num == ps_dpb_mgr->ai4_poc_buf_id_map[j][2])
 break;
 }

 if(MAX_FRAMES != i)
 {
 if(j == MAX_FRAMES)
 {
            UWORD32 i4_error_code;
            i4_error_code = ERROR_DBP_MANAGER_T;
 return i4_error_code;
 }

        ps_dpb_mgr->ai4_poc_buf_id_map[j][0] = -1;
        ps_dpb_mgr->ai4_poc_buf_id_map[j][1] = 0x7fffffff;
        ps_dpb_mgr->ai4_poc_buf_id_map[j][2] = GAP_FRAME_NUM;
        ps_dpb_mgr->i1_gaps_deleted++;

        ps_dpb_mgr->ai1_gaps_per_seq[i]--;
        ps_dpb_mgr->u1_num_gaps--;
 *pu1_del_node = 0;
 if(0 == ps_dpb_mgr->ai1_gaps_per_seq[i])
 {
            ps_dpb_mgr->ai4_gaps_start_frm_num[i] = INVALID_FRAME_NUM;
            ps_dpb_mgr->ai4_gaps_end_frm_num[i] = 0;
 }
 }
 else
 {
        UWORD32 i4_error_code;
        i4_error_code = ERROR_DBP_MANAGER_T;
 return i4_error_code;
 }

 return OK;
}
