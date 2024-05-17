void ih264d_unpack_coeff4x4_dc_4x4blk(tu_sblk4x4_coeff_data_t *ps_tu_4x4,
                                      WORD16 *pi2_out_coeff_data,
                                      UWORD8 *pu1_inv_scan)
{
    UWORD16 u2_sig_coeff_map = ps_tu_4x4->u2_sig_coeff_map;
    WORD32 idx;
    WORD16 *pi2_coeff_data = &ps_tu_4x4->ai2_level[0];

 while(u2_sig_coeff_map)
 {
        idx = CLZ(u2_sig_coeff_map);

        idx = 31 - idx;
        RESET_BIT(u2_sig_coeff_map,idx);

        idx = pu1_inv_scan[idx];
        pi2_out_coeff_data[idx] = *pi2_coeff_data++;

 }
}
