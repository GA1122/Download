UWORD32 ih264d_unpack_luma_coeff4x4_mb(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info,
                                    UWORD8 intra_flag)
{
    UWORD8 u1_mb_type = ps_cur_mb_info->u1_mb_type;
    UWORD16 ui2_luma_csbp = ps_cur_mb_info->u2_luma_csbp;
    UWORD8 *pu1_inv_scan = ps_dec->pu1_inv_scan;
    WORD16 *pi2_coeff_data = ps_dec->pi2_coeff_data;

    PROFILE_DISABLE_UNPACK_LUMA()
 if(!ps_cur_mb_info->u1_tran_form8x8)
 {
        UWORD32 u4_luma_dc_only_csbp = 0;
        UWORD32 u4_temp = 0;
        WORD16* pi2_dc_val = NULL;
  
 if(intra_flag && (u1_mb_type != I_4x4_MB))
 {
 if(CHECKBIT(ps_cur_mb_info->u1_yuv_dc_block_flag,0))
 {
                pi2_dc_val = (WORD16 *)ps_dec->pv_proc_tu_coeff_data;

                ps_dec->pv_proc_tu_coeff_data = (void *)(pi2_dc_val + 16);
 }
 }

 if(ui2_luma_csbp)
 {
            pi2_coeff_data = ps_dec->pi2_coeff_data;
            u4_temp = ih264d_unpack_coeff4x4_8x8blk(ps_dec,
                                          ps_cur_mb_info,
                                          ui2_luma_csbp,
                                          pi2_coeff_data);
            u4_luma_dc_only_csbp = u4_temp;

            pi2_coeff_data += 32;

            ui2_luma_csbp = ui2_luma_csbp >> 2;
            u4_temp = ih264d_unpack_coeff4x4_8x8blk(ps_dec,
                                          ps_cur_mb_info,
                                          ui2_luma_csbp,
                                          pi2_coeff_data);

            u4_luma_dc_only_csbp |= (u4_temp << 2);

            pi2_coeff_data += 32 + 64;

            ui2_luma_csbp = ui2_luma_csbp >> 6;
            u4_temp = ih264d_unpack_coeff4x4_8x8blk(ps_dec,
                                          ps_cur_mb_info,
                                          ui2_luma_csbp,
                                          pi2_coeff_data);

            u4_luma_dc_only_csbp |= (u4_temp << 8);

            pi2_coeff_data += 32;

            ui2_luma_csbp = ui2_luma_csbp >> 2;
            u4_temp = ih264d_unpack_coeff4x4_8x8blk(ps_dec,
                                          ps_cur_mb_info,
                                          ui2_luma_csbp,
                                          pi2_coeff_data);
            u4_luma_dc_only_csbp |= (u4_temp << 10);
 }

 if(pi2_dc_val != NULL)
 {
            WORD32 i;
            pi2_coeff_data = ps_dec->pi2_coeff_data;
 for(i = 0; i < 4; i++)
 {
                pi2_coeff_data[0] = pi2_dc_val[0];
                pi2_coeff_data[4 * 16] = pi2_dc_val[4];
                pi2_coeff_data[8 * 16] = pi2_dc_val[8];
                pi2_coeff_data[12 * 16] = pi2_dc_val[12];

                pi2_dc_val++;  
                pi2_coeff_data += 16;
 }
            u4_luma_dc_only_csbp = ps_cur_mb_info->u2_luma_csbp ^ 0xFFFF;
 }
 return u4_luma_dc_only_csbp;
 }
 else
 {
        UWORD32 u4_luma_dc_only_cbp = 0;
        WORD32 dc_only_flag;
 if(ui2_luma_csbp)
 {
            pi2_coeff_data = ps_dec->pi2_coeff_data;
            dc_only_flag = ih264d_unpack_coeff8x8_8x8blk_cavlc(ps_dec,
                                          ps_cur_mb_info,
                                          ui2_luma_csbp,
                                          pi2_coeff_data);
            INSERT_BIT(u4_luma_dc_only_cbp, 0, dc_only_flag);

            pi2_coeff_data += 64;

            ui2_luma_csbp = ui2_luma_csbp >> 2;
            dc_only_flag = ih264d_unpack_coeff8x8_8x8blk_cavlc(ps_dec,
                                          ps_cur_mb_info,
                                          ui2_luma_csbp,
                                          pi2_coeff_data);

            INSERT_BIT(u4_luma_dc_only_cbp, 1, dc_only_flag);

            pi2_coeff_data += 64;

            ui2_luma_csbp = ui2_luma_csbp >> 6;
            dc_only_flag = ih264d_unpack_coeff8x8_8x8blk_cavlc(ps_dec,
                                          ps_cur_mb_info,
                                          ui2_luma_csbp,
                                          pi2_coeff_data);

            INSERT_BIT(u4_luma_dc_only_cbp, 2, dc_only_flag);

            pi2_coeff_data += 64;
            ui2_luma_csbp = ui2_luma_csbp >> 2;
            dc_only_flag = ih264d_unpack_coeff8x8_8x8blk_cavlc(ps_dec,
                                          ps_cur_mb_info,
                                          ui2_luma_csbp,
                                          pi2_coeff_data);
            INSERT_BIT(u4_luma_dc_only_cbp, 3, dc_only_flag);
 }
 return u4_luma_dc_only_cbp;
 }

}
