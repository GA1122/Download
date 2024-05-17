void ih264d_get_implicit_weights(dec_struct_t *ps_dec)
{
    UWORD32 *pu4_iwt_ofst;
    UWORD8 i, j;
 struct pic_buffer_t *ps_pic_buff0, *ps_pic_buff1;
    WORD16 i2_dist_scale_factor;
    WORD16 i16_tb, i16_td, i16_tx;
    UWORD32 u4_poc0, u4_poc1;
    UWORD32 ui_temp0, ui_temp1;
    UWORD8 uc_num_ref_idx_l0_active, uc_num_ref_idx_l1_active;

    pu4_iwt_ofst = ps_dec->pu4_wts_ofsts_mat;
    uc_num_ref_idx_l0_active =
                    ps_dec->ps_cur_slice->u1_num_ref_idx_lx_active[0];
    uc_num_ref_idx_l1_active =
                    ps_dec->ps_cur_slice->u1_num_ref_idx_lx_active[1];

 for(i = 0; i < uc_num_ref_idx_l0_active; i++)
 {
        ps_pic_buff0 = ps_dec->ps_ref_pic_buf_lx[0][i];
        u4_poc0 = ps_pic_buff0->i4_avg_poc;
 for(j = 0; j < uc_num_ref_idx_l1_active; j++)
 {
            ps_pic_buff1 = ps_dec->ps_ref_pic_buf_lx[1][j];
            u4_poc1 = ps_pic_buff1->i4_avg_poc;

 if(u4_poc1 != u4_poc0)
 {
                i16_tb = ps_dec->ps_cur_pic->i4_poc - u4_poc0;
                i16_tb = CLIP3(-128, 127, i16_tb);
                i16_td = u4_poc1 - u4_poc0;
                i16_td = CLIP3(-128, 127, i16_td);
                i16_tx = (16384 + ABS(SIGN_POW2_DIV(i16_td, 1))) / i16_td;
                i2_dist_scale_factor = CLIP3(-1024, 1023,
 (((i16_tb * i16_tx) + 32) >> 6));

 if( 
 (!(ps_pic_buff1->u1_is_short && ps_pic_buff0->u1_is_short))
 || ((i2_dist_scale_factor >> 2) < -64)
 || ((i2_dist_scale_factor >> 2) > 128))
 {
  
                    ui_temp0 = 0x00000020;
                    ui_temp1 = 0x00000020;
 }
 else
 {
                    ui_temp0 = 64 - (i2_dist_scale_factor >> 2);
                    ui_temp1 = (i2_dist_scale_factor >> 2);
 }
 }
 else
 {
                ui_temp0 = 0x00000020;
                ui_temp1 = 0x00000020;
 }
            pu4_iwt_ofst[0] = pu4_iwt_ofst[2] = pu4_iwt_ofst[4] = ui_temp0;
            pu4_iwt_ofst[1] = pu4_iwt_ofst[3] = pu4_iwt_ofst[5] = ui_temp1;
            pu4_iwt_ofst += 6;
 }
 }
 if(ps_dec->ps_cur_slice->u1_mbaff_frame_flag)
 {
        UWORD8 k;
        WORD32 i4_cur_poc = ps_dec->ps_cur_pic->i4_top_field_order_cnt;
        UWORD32* pu4_wt_mat = ps_dec->pu4_mbaff_wt_mat;
  
 for(k = 0; k < 2; k++)
 {
 for(i = 0; i < (uc_num_ref_idx_l0_active << 1); i++)
 {
                UWORD16 u2_l0_idx;

  

                u2_l0_idx = i >> 1;
 if((i & 0x01) != k)
 {
                    u2_l0_idx += MAX_REF_BUFS;
 }
                ps_pic_buff0 = ps_dec->ps_ref_pic_buf_lx[0][u2_l0_idx];
                u4_poc0 = ps_pic_buff0->i4_poc;
 for(j = 0; j < (uc_num_ref_idx_l1_active << 1); j++)
 {
                    UWORD16 u2_l1_idx;
  

                    u2_l1_idx = j >> 1;
 if((j & 0x01) != k)
 {
                        u2_l1_idx += MAX_REF_BUFS;
 }
                    ps_pic_buff1 = ps_dec->ps_ref_pic_buf_lx[1][u2_l1_idx];
                    u4_poc1 = ps_pic_buff1->i4_poc;
 if(u4_poc1 != u4_poc0)
 {
                        i16_tb = i4_cur_poc - u4_poc0;
                        i16_tb = CLIP3(-128, 127, i16_tb);
                        i16_td = u4_poc1 - u4_poc0;
                        i16_td = CLIP3(-128, 127, i16_td);
                        i16_tx = (16384 + ABS(SIGN_POW2_DIV(i16_td, 1)))
 / i16_td;
                        i2_dist_scale_factor = CLIP3(
 -1024, 1023,
 (((i16_tb * i16_tx) + 32) >> 6));

 if( 
 (!(ps_pic_buff1->u1_is_short && ps_pic_buff0->u1_is_short))
 || ((i2_dist_scale_factor >> 2) < -64)
 || ((i2_dist_scale_factor >> 2) > 128))
 {
  
                            ui_temp0 = 0x00000020;
                            ui_temp1 = 0x00000020;
 }
 else
 {
                            ui_temp0 = 64 - (i2_dist_scale_factor >> 2);
                            ui_temp1 = (i2_dist_scale_factor >> 2);
 }
 }
 else
 {
                        ui_temp0 = 0x00000020;
                        ui_temp1 = 0x00000020;
 }
  
 *pu4_wt_mat++ = ui_temp0;
 *pu4_wt_mat++ = ui_temp1;
 *pu4_wt_mat++ = ui_temp0;
 *pu4_wt_mat++ = ui_temp1;
 *pu4_wt_mat++ = ui_temp0;
 *pu4_wt_mat++ = ui_temp1;

 }
 }
            i4_cur_poc = ps_dec->ps_cur_pic->i4_bottom_field_order_cnt;
 }
 }
}
