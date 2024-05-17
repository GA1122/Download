IMPEG2D_ERROR_CODES_T impeg2d_vld_inv_quant_mpeg2(
 void *pv_dec,  
                             WORD16       *pi2_out_addr,  
 const UWORD8 *pu1_scan,  
                             UWORD16      u2_intra_flag,  
                             UWORD16      u2_colr_comp,  
                             UWORD16      u2_d_picture         
 )
{
    UWORD8  *pu1_weighting_matrix;
    WORD32 u4_sum_is_even;
 dec_state_t *ps_dec = (dec_state_t *)pv_dec;
    IMPEG2D_ERROR_CODES_T e_error = (IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE;

    WORD16  pi2_coeffs[NUM_COEFFS];
    UWORD8  pi4_pos[NUM_COEFFS];
    WORD32  i4_num_coeffs;

  
    e_error = impeg2d_vld_decode(ps_dec, pi2_coeffs, pu1_scan, pi4_pos, u2_intra_flag,
                                 u2_colr_comp, u2_d_picture, ps_dec->u2_intra_vlc_format,
                                 ps_dec->u2_is_mpeg2, &i4_num_coeffs);
 if ((IMPEG2D_ERROR_CODES_T)IVD_ERROR_NONE != e_error)
 {
 return e_error;
 }

  
    pu1_weighting_matrix = (u2_intra_flag == 1) ? ps_dec->au1_intra_quant_matrix:
                    ps_dec->au1_inter_quant_matrix;

  
  
    ps_dec->i4_last_value_one = 0;

    IMPEG2D_IQNT_INP_STATISTICS(pi2_out_addr, ps_dec->u4_non_zero_cols, ps_dec->u4_non_zero_rows);

  
    PROFILE_DISABLE_INVQUANT_IF0

 {
  
        PROFILE_DISABLE_MEMSET_RESBUF_IF0
 if (1 != (ps_dec->u4_non_zero_cols | ps_dec->u4_non_zero_rows))
 {
            ps_dec->pf_memset_16bit_8x8_linear_block (pi2_out_addr);
 }

        u4_sum_is_even  = impeg2d_inv_quant_mpeg2(pi2_out_addr, pu1_weighting_matrix,
                                                 ps_dec->u1_quant_scale, u2_intra_flag,
                                                 i4_num_coeffs, pi2_coeffs,
                                                 pi4_pos, pu1_scan,
 &ps_dec->u2_def_dc_pred[u2_colr_comp],
                                                 ps_dec->u2_intra_dc_precision);

 if (0 != pi2_out_addr[0])
 {
  
            ps_dec->u4_non_zero_cols  |= 0x1;
            ps_dec->u4_non_zero_rows  |= 0x1;
 }

 if (1 == (ps_dec->u4_non_zero_cols | ps_dec->u4_non_zero_rows))
 {
            ps_dec->i4_last_value_one = 1 - (pi2_out_addr[0] & 1);
 }
 else
 {
  
            pi2_out_addr[63] ^= (u4_sum_is_even & 1);

 if (0 != pi2_out_addr[63])
 {
                ps_dec->u4_non_zero_cols  |= 0x80;
                ps_dec->u4_non_zero_rows  |= 0x80;
 }
 }
 }

 return e_error;
}
