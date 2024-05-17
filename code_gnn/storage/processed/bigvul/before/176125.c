VOID ixheaacd_esbr_cos_sin_mod(WORD32 *subband,
                               ia_sbr_qmf_filter_bank_struct *qmf_bank,
                               WORD32 *p_twiddle, WORD32 *p_dig_rev_tbl) {
  WORD32 z;
  WORD32 temp[128];
  WORD32 scaleshift = 0;

  WORD32 re2, re3;
  WORD32 wim, wre;

  WORD32 i, M_2;
  WORD32 M = ixheaacd_shr32(qmf_bank->no_channels, 1);

 const WORD32 *p_sin;
 const WORD32 *p_sin_cos;

  WORD32 subband_tmp[128];
  WORD32 re;
  WORD32 im;
  WORD32 *psubband, *psubband1;
  WORD32 *psubband_t, *psubband1_t;
  WORD32 *psubband2, *psubband12;
  WORD32 *psubband_t2, *psubband1_t2;

  M_2 = ixheaacd_shr32(M, 1);

  p_sin_cos = qmf_bank->esbr_cos_twiddle;

  psubband = &subband[0];
  psubband1 = &subband[2 * M - 1];
 psubband_t = subband_tmp;
 psubband1_t = &subband_tmp[2 * M - 1];

  psubband2 = &subband[64];
  psubband12 = &subband[2 * M - 1 + 64];
  psubband_t2 = &subband_tmp[64];
  psubband1_t2 = &subband_tmp[2 * M - 1 + 64];

 for (i = (M_2 >> 1) - 1; i >= 0; i--) {
    re = *psubband++;
    im = *psubband1--;

    wim = *p_sin_cos++;
    wre = *p_sin_cos++;

 *psubband_t++ = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wre), ixheaacd_mult64(im, wim))) >>
 32);
 *psubband_t++ = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wre),
                                                 ixheaacd_mult64(re, wim))) >>
 32);

    re = *psubband2++;
    im = *psubband12--;

 *psubband_t2++ = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wim),
                                                  ixheaacd_mult64(re, wre))) >>
 32);
 *psubband_t2++ = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wim), ixheaacd_mult64(im, wre))) >>
 32);

    re = *psubband1--;
    im = *psubband++;

    wim = *p_sin_cos++;
    wre = *p_sin_cos++;

 *psubband1_t-- = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wre),
                                                  ixheaacd_mult64(re, wim))) >>
 32);
 *psubband1_t-- = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wre), ixheaacd_mult64(im, wim))) >>
 32);

    re = *psubband12--;
    im = *psubband2++;

 *psubband1_t2-- = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wim), ixheaacd_mult64(im, wre))) >>
 32);
 *psubband1_t2-- = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wim),
                                                   ixheaacd_mult64(re, wre))) >>
 32);

    re = *psubband++;
    im = *psubband1--;

    wim = *p_sin_cos++;
    wre = *p_sin_cos++;

 *psubband_t++ = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wre), ixheaacd_mult64(im, wim))) >>
 32);
 *psubband_t++ = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wre),
                                                 ixheaacd_mult64(re, wim))) >>
 32);

    re = *psubband2++;
    im = *psubband12--;

 *psubband_t2++ = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wim),
                                                  ixheaacd_mult64(re, wre))) >>
 32);
 *psubband_t2++ = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wim), ixheaacd_mult64(im, wre))) >>
 32);

    re = *psubband1--;
    im = *psubband++;
 ;

    wim = *p_sin_cos++;
    wre = *p_sin_cos++;

 *psubband1_t-- = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wre),
                                                  ixheaacd_mult64(re, wim))) >>
 32);
 *psubband1_t-- = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wre), ixheaacd_mult64(im, wim))) >>
 32);

    re = *psubband12--;
    im = *psubband2++;
 ;

 *psubband1_t2-- = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wim), ixheaacd_mult64(im, wre))) >>
 32);
 *psubband1_t2-- = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wim),
                                                   ixheaacd_mult64(re, wre))) >>
 32);
 }

 if (M == 32) {
    ixheaacd_esbr_radix4bfly(p_twiddle, subband_tmp, 1, 8);
    ixheaacd_esbr_radix4bfly(p_twiddle + 48, subband_tmp, 4, 2);
    ixheaacd_esbr_postradixcompute2(subband, subband_tmp, p_dig_rev_tbl, 32);

    ixheaacd_esbr_radix4bfly(p_twiddle, &subband_tmp[64], 1, 8);
    ixheaacd_esbr_radix4bfly(p_twiddle + 48, &subband_tmp[64], 4, 2);
    ixheaacd_esbr_postradixcompute2(&subband[64], &subband_tmp[64],
                                    p_dig_rev_tbl, 32);

 }

 else if (M == 16) {
    ixheaacd_esbr_radix4bfly(p_twiddle, subband_tmp, 1, 4);
    ixheaacd_esbr_postradixcompute4(subband, subband_tmp, p_dig_rev_tbl, 16);

    ixheaacd_esbr_radix4bfly(p_twiddle, &subband_tmp[64], 1, 4);
    ixheaacd_esbr_postradixcompute4(&subband[64], &subband_tmp[64],
                                    p_dig_rev_tbl, 16);

 }

 else if (M == 12) {
 for (z = 0; z < (qmf_bank->no_channels >> 1); z++) {
      temp[z] = subband_tmp[2 * z];
      temp[12 + z] = subband_tmp[2 * z + 1];
 }

    ixheaacd_complex_fft_p3(temp, &temp[12], 12, -1, &scaleshift);

 for (z = 0; z < (qmf_bank->no_channels >> 1); z++) {
      subband[2 * z] = temp[z];
      subband[2 * z + 1] = temp[z + 12];
 }
    scaleshift = 0;
 for (z = 0; z < (qmf_bank->no_channels >> 1); z++) {
      temp[z] = subband_tmp[64 + 2 * z];
      temp[12 + z] = subband_tmp[64 + 2 * z + 1];
 }

    ixheaacd_complex_fft_p3(temp, &temp[12], 12, -1, &scaleshift);

 for (z = 0; z < (qmf_bank->no_channels >> 1); z++) {
      subband[64 + 2 * z] = temp[z];
      subband[64 + 2 * z + 1] = temp[z + 12];
 }

 }

 else {
 for (z = 0; z < (qmf_bank->no_channels >> 1); z++) {
      temp[z] = subband_tmp[2 * z];
      temp[8 + z] = subband_tmp[2 * z + 1];
 }

 (*ixheaacd_complex_fft_p2)(temp, &temp[8], 8, -1, &scaleshift);

 for (z = 0; z < (qmf_bank->no_channels >> 1); z++) {
      subband[2 * z] = temp[z] << scaleshift;
      subband[2 * z + 1] = temp[z + 8] << scaleshift;
 }
    scaleshift = 0;
 for (z = 0; z < (qmf_bank->no_channels >> 1); z++) {
      temp[z] = subband_tmp[64 + 2 * z];
      temp[8 + z] = subband_tmp[64 + 2 * z + 1];
 }

 (*ixheaacd_complex_fft_p2)(temp, &temp[8], 8, -1, &scaleshift);

 for (z = 0; z < (qmf_bank->no_channels >> 1); z++) {
      subband[64 + 2 * z] = temp[z] << scaleshift;
      subband[64 + 2 * z + 1] = temp[8 + z] << scaleshift;
 }
 }

  psubband = &subband[0];
  psubband1 = &subband[2 * M - 1];

  re = *psubband1;

 *psubband = *psubband >> 1;
  psubband++;
 *psubband1 = ixheaacd_negate32(*psubband >> 1);
  psubband1--;

  p_sin = qmf_bank->esbr_alt_sin_twiddle;

  wim = *p_sin++;
  wre = *p_sin++;

  im = *psubband1;
 ;

 *psubband1-- = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wre), ixheaacd_mult64(im, wim))) >>
 32);
 *psubband++ = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wre),
                                             ixheaacd_mult64(re, wim))) >>
 32);

  psubband2 = &subband[64];
  psubband12 = &subband[2 * M - 1 + 64];

  re = *psubband12;
 ;

 *psubband12-- = ixheaacd_negate32_sat(*psubband2 >> 1);
 ;
 *psubband2 = psubband2[1] >> 1;
 ;
  psubband2++;

  im = *psubband12;
 ;

 *psubband2++ = ixheaacd_negate32_sat((WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wre), ixheaacd_mult64(im, wim))) >>
 32));
 *psubband12-- = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(re, wim),
                                               ixheaacd_mult64(im, wre))) >>
 32);

 for (i = (M_2 - 2); i >= 0; i--) {
    im = psubband[0];
 ;
    re = psubband[1];
 ;
    re2 = *psubband1;
 ;

 *psubband++ = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wim), ixheaacd_mult64(im, wre))) >>
 32);
 *psubband1-- = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wim),
                                                ixheaacd_mult64(re, wre))) >>
 32);

    im = psubband2[0];
 ;
    re = psubband2[1];
 ;
    re3 = *psubband12;
 ;

 *psubband12-- = ixheaacd_negate32_sat((WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re, wim), ixheaacd_mult64(im, wre))) >>
 32));
 *psubband2++ = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(re, wre),
                                                ixheaacd_mult64(im, wim))) >>
 32);

    wim = *p_sin++;
    wre = *p_sin++;
    im = psubband1[0];
 ;

 *psubband1-- = (WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re2, wre), ixheaacd_mult64(im, wim))) >>
 32);
 *psubband++ = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(im, wre),
                                               ixheaacd_mult64(re2, wim))) >>
 32);

    im = psubband12[0];
 ;

 *psubband2++ = ixheaacd_negate32_sat((WORD32)(
 (ixheaacd_add64(ixheaacd_mult64(re3, wre), ixheaacd_mult64(im, wim))) >>
 32));
 *psubband12-- = (WORD32)((ixheaacd_sub64_sat(ixheaacd_mult64(re3, wim),
                                                 ixheaacd_mult64(im, wre))) >>
 32);
 }
}
