VOID ixheaacd_esbr_fwd_modulation(
 const WORD32 *time_sample_buf, WORD32 *real_subband, WORD32 *imag_subband,
    ia_sbr_qmf_filter_bank_struct *qmf_bank,
    ia_qmf_dec_tables_struct *qmf_dec_tables_ptr) {
  WORD32 i;
 const WORD32 *time_sample_buf1 =
 &time_sample_buf[2 * qmf_bank->no_channels - 1];
  WORD32 temp1, temp2;
  WORD32 *t_real_subband = real_subband;
  WORD32 *t_imag_subband = imag_subband;
 const WORD32 *tcos;

 for (i = qmf_bank->no_channels - 1; i >= 0; i--) {
    temp1 = ixheaacd_shr32(*time_sample_buf++, HQ_SHIFT_64);
    temp2 = ixheaacd_shr32(*time_sample_buf1--, HQ_SHIFT_64);

 *t_real_subband++ = ixheaacd_sub32_sat(temp1, temp2);
 ;
 *t_imag_subband++ = ixheaacd_add32(temp1, temp2);
 ;
 }

  ixheaacd_esbr_cos_sin_mod(real_subband, qmf_bank,
                            qmf_dec_tables_ptr->esbr_w_16,
                            qmf_dec_tables_ptr->dig_rev_table4_16);

  tcos = qmf_bank->esbr_t_cos;

 for (i = (qmf_bank->usb - qmf_bank->lsb - 1); i >= 0; i--) {
    WORD32 cosh, sinh;
    WORD32 re, im;

    re = *real_subband;
    im = *imag_subband;
    cosh = *tcos++;
    sinh = *tcos++;
 *real_subband++ = (WORD32)((ixheaacd_add64(ixheaacd_mult64(re, cosh),
                                               ixheaacd_mult64(im, sinh))) >>
 31);
 *imag_subband++ =
 (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, cosh),
                                     ixheaacd_mult64(re, sinh))) >>
 31);
 }
}
