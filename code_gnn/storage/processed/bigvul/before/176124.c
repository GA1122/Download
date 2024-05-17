VOID ixheaacd_dct3_32(WORD32 *input, WORD32 *output,
 const WORD16 *main_twidle_fwd, const WORD16 *post_tbl,
 const WORD16 *w_16, const WORD32 *p_table) {
  WORD32 n, k;

  WORD32 temp1[6];
  WORD32 temp2[4];
  WORD16 twid_re, twid_im;
  WORD32 *ptr_reverse, *ptr_forward, *p_out, *ptr_out1;
 const WORD16 *twidle_fwd, *twidle_rev;

  ptr_forward = &input[49];
  ptr_reverse = &input[47];

  p_out = output;
  twidle_fwd = main_twidle_fwd;
  twidle_fwd += 4;

 *p_out++ = input[48] >> LP_SHIFT_VAL;
 *p_out++ = 0;

 for (n = 1; n < DCT3_LEN / 2; n++) {
    temp1[0] = *ptr_forward++;
    temp1[1] = *ptr_reverse--;
    temp1[0] = ixheaacd_add32(ixheaacd_shr32(temp1[0], LP_SHIFT_VAL),
                              ixheaacd_shr32(temp1[1], LP_SHIFT_VAL));

    temp1[2] = *(ptr_forward - 33);
    temp1[3] = *(ptr_reverse - 31);
    temp1[1] = ixheaacd_sub32(ixheaacd_shr32(temp1[2], LP_SHIFT_VAL),
                              ixheaacd_shr32(temp1[3], LP_SHIFT_VAL));
    twid_re = *twidle_fwd++;

    twid_im = *twidle_fwd;
    twidle_fwd += 3;
 *p_out++ = mac32x16in32_dual(temp1[0], twid_re, temp1[1], twid_im);
 *p_out++ = msu32x16in32_dual(temp1[0], twid_im, temp1[1], twid_re);
 }
  twid_re = *twidle_fwd++;

  twid_im = *twidle_fwd;
  twidle_fwd += 3;

  temp1[1] = *ptr_reverse--;
  temp1[0] = *(ptr_reverse - 31);
  temp1[1] = ixheaacd_sub32(ixheaacd_shr32(temp1[1], LP_SHIFT_VAL),
                            ixheaacd_shr32(temp1[0], LP_SHIFT_VAL));

  temp1[0] = temp1[1];

  temp2[2] = mac32x16in32_dual(temp1[0], twid_re, temp1[1], twid_im);
  temp2[3] = msu32x16in32_dual(temp1[0], twid_im, temp1[1], twid_re);

  ptr_forward = output;
  ptr_reverse = &output[DCT3_LEN - 1];
  temp2[0] = *ptr_forward++;
  temp2[1] = *ptr_forward--;

  temp1[0] = -temp2[1] - temp2[3];
  temp1[1] = temp2[0] - temp2[2];
  temp2[0] = (temp2[0] + temp2[2] + temp1[0]);
  temp2[1] = (temp2[1] - temp2[3] + temp1[1]);

  temp2[0] >>= 1;
  temp2[1] >>= 1;

 *ptr_forward++ = temp2[0];
 *ptr_forward++ = temp2[1];

  twidle_fwd = post_tbl + 2;
  twidle_rev = post_tbl + 14;

 for (n = 1; n < DCT3_LEN / 4; n++) {
    temp2[0] = *ptr_forward++;
    temp2[1] = *ptr_forward--;
    temp2[3] = *ptr_reverse--;
    temp2[2] = *ptr_reverse++;

    twid_re = *twidle_rev;
    twidle_rev -= 2;
    twid_im = *twidle_fwd;
    twidle_fwd += 2;

    temp1[0] = temp2[0] - temp2[2];
    temp1[1] = (temp2[0] + temp2[2]);

    temp1[2] = temp2[1] + temp2[3];
    temp1[3] = (temp2[1] - temp2[3]);
    temp1[4] = mac32x16in32_dual(temp1[0], twid_re, temp1[2], twid_im);
    temp1[5] = msu32x16in32_dual(temp1[0], twid_im, temp1[2], twid_re);

    temp1[1] >>= 1;
    temp1[3] >>= 1;

 *ptr_forward++ = temp1[1] - temp1[4];
 *ptr_forward++ = temp1[3] + temp1[5];

 *ptr_reverse-- = -temp1[3] + temp1[5];
 *ptr_reverse-- = temp1[1] + temp1[4];
 }
  temp2[0] = *ptr_forward++;
  temp2[1] = *ptr_forward--;
  temp2[3] = *ptr_reverse--;
  temp2[2] = *ptr_reverse++;

  twid_re = *twidle_rev;
  twidle_rev -= 2;
  twid_im = *twidle_fwd;
  twidle_fwd += 2;

  temp1[0] = temp2[0] - temp2[2];
  temp1[1] = (temp2[0] + temp2[2]);

  temp1[2] = temp2[1] + temp2[3];
  temp1[3] = (temp2[1] - temp2[3]);

  temp1[4] = -mac32x16in32_dual(temp1[0], twid_re, temp1[2], twid_im);
  temp1[5] = msu32x16in32_dual(temp1[0], twid_im, temp1[2], twid_re);

  temp1[1] >>= 1;
  temp1[3] >>= 1;
 *ptr_forward++ = temp1[1] + temp1[4];
 *ptr_forward++ = temp1[3] + temp1[5];

  ixheaacd_radix4bfly(w_16, output, 1, 4);
  ixheaacd_postradixcompute4(input, output, p_table, 16);

  output[0] = input[0];
  output[2] = input[1];

  p_out = input + 2;
  ptr_forward = output + 1;
  ptr_reverse = output + 30;
  ptr_out1 = input + 18;

 for (k = (DCT3_LEN / 4) - 1; k != 0; k--) {
    WORD32 tempre, tempim;

    tempre = *p_out++;
    tempim = *p_out++;
 *ptr_forward = (tempim);
    ptr_forward += 2;
 *ptr_forward = (tempre);
    ptr_forward += 2;

    tempre = *ptr_out1++;
    tempim = *ptr_out1++;
 *ptr_reverse = (tempim);
    ptr_reverse -= 2;
 *ptr_reverse = (tempre);
    ptr_reverse -= 2;
 }

 {
    WORD32 tempre, tempim;
    tempre = *p_out++;
    tempim = *p_out++;
 *ptr_forward = (tempim);
    ptr_forward += 2;
 *ptr_forward = (tempre);
    ptr_forward += 2;
 }

 return;
}
