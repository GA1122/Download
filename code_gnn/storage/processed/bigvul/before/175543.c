IHEVCD_ERROR_T ihevcd_get_tile_pos(pps_t *ps_pps,
 sps_t *ps_sps,
                                   WORD32 ctb_x,
                                   WORD32 ctb_y,
                                   WORD32 *pi4_ctb_tile_x,
                                   WORD32 *pi4_ctb_tile_y,
                                   WORD32 *pi4_tile_idx)
{

 tile_t *ps_tile_tmp;
    WORD32 i;
    WORD32 tile_row, tile_col;

 if(ctb_x < 0 || ctb_y < 0)
 {
 *pi4_ctb_tile_x = 0;
 *pi4_ctb_tile_y = 0;
 *pi4_tile_idx = 0;

 return (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
 }

    tile_row = 0;
    tile_col = 0;
    ps_tile_tmp = ps_pps->ps_tile;
 if(0 == ps_pps->i1_tiles_enabled_flag)
 {
 *pi4_ctb_tile_x = ctb_x;
 *pi4_ctb_tile_y = ctb_y;
 *pi4_tile_idx = 0;
 }
 else
 {
 for(i = 0; i < ps_pps->i1_num_tile_columns; i++)
 {
            WORD16 next_tile_ctb_x;
            ps_tile_tmp = ps_pps->ps_tile + i;  
 if((ps_pps->i1_num_tile_columns - 1) == i)
 {
                next_tile_ctb_x = ps_sps->i2_pic_wd_in_ctb;
 }
 else
 {
 tile_t *ps_tile_next_tmp;
                ps_tile_next_tmp = ps_pps->ps_tile + i + 1;
                next_tile_ctb_x = ps_tile_next_tmp->u1_pos_x;
 }
 if((ctb_x >= ps_tile_tmp->u1_pos_x) && (ctb_x < next_tile_ctb_x))
 {
                tile_col = i;
 break;
 }
 }
 *pi4_ctb_tile_x = ctb_x - ps_tile_tmp->u1_pos_x;

 for(i = 0; i < ps_pps->i1_num_tile_rows; i++)
 {
            WORD16 next_tile_ctb_y;
            ps_tile_tmp = ps_pps->ps_tile + i * ps_pps->i1_num_tile_columns;
 if((ps_pps->i1_num_tile_rows - 1) == i)
 {
                next_tile_ctb_y = ps_sps->i2_pic_ht_in_ctb;
 }
 else
 {
 tile_t *ps_tile_next_tmp;
                ps_tile_next_tmp = ps_pps->ps_tile + ((i + 1) * ps_pps->i1_num_tile_columns);
                next_tile_ctb_y = ps_tile_next_tmp->u1_pos_y;
 }
 if((ctb_y >= ps_tile_tmp->u1_pos_y) && (ctb_y < next_tile_ctb_y))
 {
                tile_row = i;
 break;
 }

 }
 *pi4_ctb_tile_y = ctb_y - ps_tile_tmp->u1_pos_y;
 *pi4_tile_idx = tile_row * ps_pps->i1_num_tile_columns
 + tile_col;
 }
 return (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
}
