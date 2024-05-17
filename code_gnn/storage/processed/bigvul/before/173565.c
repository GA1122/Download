UWORD32 ih264d_unpack_luma_coeff8x8_mb(dec_struct_t * ps_dec,
 dec_mb_info_t * ps_cur_mb_info)
{
    WORD32 blk_8x8_cnt;
    WORD16 *pi2_out_coeff_data = ps_dec->pi2_coeff_data;
    UWORD8 u1_field_coding_flag = ps_cur_mb_info->ps_curmb->u1_mb_fld;
    UWORD8 *pu1_inv_scan;
    UWORD32 u4_luma_dc_only_cbp = 0;

    PROFILE_DISABLE_UNPACK_LUMA()
 if(!u1_field_coding_flag)
 {
  
  
  
        pu1_inv_scan = (UWORD8 *)gau1_ih264d_inv_scan_prog8x8_cabac;
 }
 else
 {
  
  
  
        pu1_inv_scan = (UWORD8 *)gau1_ih264d_inv_scan_int8x8_cabac;
 }

 for(blk_8x8_cnt = 0; blk_8x8_cnt < 4; blk_8x8_cnt++)
 {
 if(CHECKBIT(ps_cur_mb_info->u1_cbp, blk_8x8_cnt))
 {
 tu_blk8x8_coeff_data_t *ps_tu_8x8 = (tu_blk8x8_coeff_data_t *)ps_dec->pv_proc_tu_coeff_data;
            UWORD32 u4_sig_coeff_map;
            WORD32 idx = 0;
            WORD16 *pi2_coeff_data = &ps_tu_8x8->ai2_level[0];
            WORD32 num_coeff = 0;

  
            memset(pi2_out_coeff_data,0,64*sizeof(WORD16));

            u4_sig_coeff_map = ps_tu_8x8->au4_sig_coeff_map[1];

 while(u4_sig_coeff_map)
 {
                idx = CLZ(u4_sig_coeff_map);

                idx = 31 - idx;
                RESET_BIT(u4_sig_coeff_map,idx);

                idx = pu1_inv_scan[idx + 32];
                pi2_out_coeff_data[idx] = *pi2_coeff_data++;
                num_coeff++;
 }

            u4_sig_coeff_map = ps_tu_8x8->au4_sig_coeff_map[0];
 while(u4_sig_coeff_map)
 {
                idx = CLZ(u4_sig_coeff_map);

                idx = 31 - idx;
                RESET_BIT(u4_sig_coeff_map,idx);

                idx = pu1_inv_scan[idx];
                pi2_out_coeff_data[idx] = *pi2_coeff_data++;
                num_coeff++;
 }

 if((num_coeff == 1) && (idx == 0))
 {
                SET_BIT(u4_luma_dc_only_cbp,blk_8x8_cnt);
 }


 {
                WORD32 offset;
                offset = (UWORD8 *)pi2_coeff_data - (UWORD8 *)ps_tu_8x8;
                offset = ALIGN4(offset);
                ps_dec->pv_proc_tu_coeff_data = (void *)((UWORD8 *)ps_dec->pv_proc_tu_coeff_data + offset);
 }
 }
        pi2_out_coeff_data += 64;
 }

 return u4_luma_dc_only_cbp;
}
