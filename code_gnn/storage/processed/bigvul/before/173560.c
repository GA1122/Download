WORD32 ih264d_unpack_coeff4x4_4x4blk(dec_struct_t * ps_dec,
                                   WORD16 *pi2_out_coeff_data,
                                   UWORD8 *pu1_inv_scan)
{
 tu_sblk4x4_coeff_data_t *ps_tu_4x4 = (tu_sblk4x4_coeff_data_t *)ps_dec->pv_proc_tu_coeff_data;
    UWORD16 u2_sig_coeff_map = ps_tu_4x4->u2_sig_coeff_map;
    WORD32 idx = 0;
    WORD16 *pi2_coeff_data = &ps_tu_4x4->ai2_level[0];
    WORD32 dc_only_flag = 0;
    WORD32 num_coeff = 0;

    PROFILE_DISABLE_UNPACK_LUMA()
 while(u2_sig_coeff_map)
 {
        idx = CLZ(u2_sig_coeff_map);

        idx = 31 - idx;
        RESET_BIT(u2_sig_coeff_map,idx);

        idx = pu1_inv_scan[idx];
        pi2_out_coeff_data[idx] = *pi2_coeff_data++;
        num_coeff++;
 }

 if((num_coeff == 1) && (idx == 0))
 {
        dc_only_flag = 1;
 }

 {
        WORD32 offset;
        offset = (UWORD8 *)pi2_coeff_data - (UWORD8 *)ps_tu_4x4;
        offset = ALIGN4(offset);
        ps_dec->pv_proc_tu_coeff_data = (void *)((UWORD8 *)ps_dec->pv_proc_tu_coeff_data + offset);
 }

 return dc_only_flag;
}
