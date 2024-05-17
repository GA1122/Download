WORD32 ih264d_insert_pic_in_display_list(dpb_manager_t *ps_dpb_mgr,
                                         UWORD8 u1_buf_id,
                                         WORD32 i4_display_poc,
                                         UWORD32 u4_frame_num)
{
    WORD8 i;
    WORD32 (*i4_poc_buf_id_map)[3] = ps_dpb_mgr->ai4_poc_buf_id_map;

 for(i = 0; i < MAX_FRAMES; i++)
 {
  
 if(i4_poc_buf_id_map[i][0] == -1)
 {
 if(GAP_FRAME_NUM == i4_poc_buf_id_map[i][2])
                ps_dpb_mgr->i1_gaps_deleted--;
 else
                ps_dpb_mgr->i1_poc_buf_id_entries++;

            i4_poc_buf_id_map[i][0] = u1_buf_id;
            i4_poc_buf_id_map[i][1] = i4_display_poc;
            i4_poc_buf_id_map[i][2] = u4_frame_num;

 break;
 }
 }

 if(MAX_FRAMES == i)
 {

        UWORD32 i4_error_code;
        i4_error_code = ERROR_GAPS_IN_FRM_NUM;
 return i4_error_code;
 }
 return OK;
}
