VOID ixheaacd_cos_sin_mod(WORD32 *subband,
                          ia_sbr_qmf_filter_bank_struct *qmf_bank,
                          WORD16 *p_twiddle, WORD32 *p_dig_rev_tbl) {
  WORD32 M = ixheaacd_shr32(qmf_bank->no_channels, 1);

 const WORD16 *p_sin;
 const WORD16 *p_sin_cos = &qmf_bank->cos_twiddle[0];
  WORD32 subband_tmp[128];

  ixheaacd_cos_sin_mod_loop1(subband, M, p_sin_cos, subband_tmp);

 if (M == 32) {
    ixheaacd_sbr_imdct_using_fft(
 (const WORD32 *)p_twiddle, 32, subband_tmp, subband,
 (UWORD8 *)p_dig_rev_tbl, (UWORD8 *)p_dig_rev_tbl,
 (UWORD8 *)p_dig_rev_tbl, (UWORD8 *)p_dig_rev_tbl);

    ixheaacd_sbr_imdct_using_fft(
 (const WORD32 *)p_twiddle, 32, &subband_tmp[64], &subband[64],
 (UWORD8 *)p_dig_rev_tbl, (UWORD8 *)p_dig_rev_tbl,
 (UWORD8 *)p_dig_rev_tbl, (UWORD8 *)p_dig_rev_tbl);

 } else {
    ixheaacd_sbr_imdct_using_fft(
 (const WORD32 *)p_twiddle, 16, subband_tmp, subband,
 (UWORD8 *)p_dig_rev_tbl, (UWORD8 *)p_dig_rev_tbl,
 (UWORD8 *)p_dig_rev_tbl, (UWORD8 *)p_dig_rev_tbl);

    ixheaacd_sbr_imdct_using_fft(
 (const WORD32 *)p_twiddle, 16, &subband_tmp[64], &subband[64],
 (UWORD8 *)p_dig_rev_tbl, (UWORD8 *)p_dig_rev_tbl,
 (UWORD8 *)p_dig_rev_tbl, (UWORD8 *)p_dig_rev_tbl);
 }

  p_sin = &qmf_bank->alt_sin_twiddle[0];
  ixheaacd_cos_sin_mod_loop2(subband, p_sin, M);
}
