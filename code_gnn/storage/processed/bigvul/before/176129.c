VOID ixheaacd_fwd_modulation(const WORD32 *p_time_in1, WORD32 *real_subband,
                             WORD32 *imag_subband,
                             ia_sbr_qmf_filter_bank_struct *qmf_bank,
                             ia_qmf_dec_tables_struct *qmf_dec_tables_ptr) {
  WORD32 i;
 const WORD32 *p_time_in2 = &p_time_in1[2 * NO_ANALYSIS_CHANNELS - 1];
  WORD32 temp1, temp2;
  WORD32 *t_real_subband = real_subband;
  WORD32 *t_imag_subband = imag_subband;
 const WORD16 *tcos;

 for (i = NO_ANALYSIS_CHANNELS - 1; i >= 0; i--) {
    temp1 = ixheaacd_shr32(*p_time_in1++, HQ_SHIFT_VAL);
    temp2 = ixheaacd_shr32(*p_time_in2--, HQ_SHIFT_VAL);

 *t_real_subband++ = ixheaacd_sub32_sat(temp1, temp2);
 ;
 *t_imag_subband++ = ixheaacd_add32(temp1, temp2);
 ;
 }

  ixheaacd_cos_sin_mod(real_subband, qmf_bank,
 (WORD16 *)qmf_dec_tables_ptr->w1024,
 (WORD32 *)qmf_dec_tables_ptr->dig_rev_table2_128);

  tcos = qmf_bank->t_cos;

 for (i = (qmf_bank->usb - qmf_bank->lsb - 1); i >= 0; i--) {
    WORD16 cosh, sinh;
    WORD32 re, im;

    re = *real_subband;
    im = *imag_subband;
    cosh = *tcos++;
    sinh = *tcos++;
 *real_subband++ = ixheaacd_add32(ixheaacd_mult32x16in32_shl(re, cosh),
                                     ixheaacd_mult32x16in32_shl(im, sinh));
 *imag_subband++ = ixheaacd_sub32_sat(ixheaacd_mult32x16in32_shl(im, cosh),
                                         ixheaacd_mult32x16in32_shl(re, sinh));
 }
}
