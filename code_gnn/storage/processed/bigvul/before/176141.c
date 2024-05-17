VOID ixheaacd_hbe_xprod_proc_3(ia_esbr_hbe_txposer_struct *ptr_hbe_txposer,
                               WORD32 qmf_band_idx, WORD32 qmf_col_idx,
                               FLOAT32 p, WORD32 pitch_in_bins_idx) {
  WORD32 tr, n1, n2, max_trans_fac, max_n1, max_n2;
  WORD32 k, addrshift;
  WORD32 inp_band_idx = 2 * qmf_band_idx / 3;

  FLOAT64 temp_fac;
  FLOAT32 max_mag_value;
  FLOAT32 mag_zero_band, mag_n1_band, mag_n2_band, temp;
  FLOAT32 temp_r, temp_i;
  FLOAT32 mag_cmplx_gain = 1.8856f;

  FLOAT32 *qmf_in_buf_ri =
      ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + HBE_ZERO_BAND_IDX];

  mag_zero_band =
      qmf_in_buf_ri[2 * inp_band_idx] * qmf_in_buf_ri[2 * inp_band_idx] +
      qmf_in_buf_ri[2 * inp_band_idx + 1] * qmf_in_buf_ri[2 * inp_band_idx + 1];
  max_mag_value = 0;
  max_n1 = max_n2 = max_trans_fac = 0;

 for (tr = 1; tr < 3; tr++) {
    temp_fac = (2.0f * qmf_band_idx + 1 - tr * p) * 0.3333334;

    n1 = (WORD32)(temp_fac);
    n2 = (WORD32)(temp_fac + p);

    mag_n1_band = qmf_in_buf_ri[2 * n1] * qmf_in_buf_ri[2 * n1] +
                  qmf_in_buf_ri[2 * n1 + 1] * qmf_in_buf_ri[2 * n1 + 1];
    mag_n2_band = qmf_in_buf_ri[2 * n2] * qmf_in_buf_ri[2 * n2] +
                  qmf_in_buf_ri[2 * n2 + 1] * qmf_in_buf_ri[2 * n2 + 1];
    temp = min(mag_n1_band, mag_n2_band);

 if (temp > max_mag_value) {
      max_mag_value = temp;
      max_trans_fac = tr;
      max_n1 = n1;
      max_n2 = n2;
 }
 }

 if (max_mag_value > mag_zero_band && max_n1 >= 0 &&
      max_n2 < NO_QMF_SYNTH_CHANNELS) {
    FLOAT32 vec_y_r[2], vec_y_i[2], vec_o_r[2], vec_o_i[2];
    FLOAT32 coeff_real[2], coeff_imag[2];
    FLOAT32 d1, d2;
    WORD32 mid_trans_fac, idx;
    FLOAT64 base = 1e-17;
    FLOAT32 mag_scaling_fac = 0;
    FLOAT32 x_zero_band_r;
    FLOAT32 x_zero_band_i;

    x_zero_band_r = 0;
    x_zero_band_i = 0;
    mid_trans_fac = 3 - max_trans_fac;
 if (max_trans_fac == 1) {
      WORD32 idx;
      d1 = 0;
      d2 = 1.5;
      x_zero_band_r = qmf_in_buf_ri[2 * max_n1];
      x_zero_band_i = qmf_in_buf_ri[2 * max_n1 + 1];

      idx = max_n2 & 3;
      idx = (idx + 1) & 3;
      coeff_real[0] = ixheaacd_hbe_post_anal_proc_interp_coeff[idx][0];
      coeff_imag[0] = ixheaacd_hbe_post_anal_proc_interp_coeff[idx][1];

      coeff_real[1] = coeff_real[0];
      coeff_imag[1] = -coeff_imag[0];

      vec_y_r[1] = qmf_in_buf_ri[2 * max_n2];
      vec_y_i[1] = qmf_in_buf_ri[2 * max_n2 + 1];

      addrshift = -2;
      temp_r = ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + addrshift +
                                           HBE_ZERO_BAND_IDX][2 * max_n2];
      temp_i = ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + addrshift +
                                           HBE_ZERO_BAND_IDX][2 * max_n2 + 1];

      vec_y_r[0] = coeff_real[1] * temp_r - coeff_imag[1] * temp_i;
      vec_y_i[0] = coeff_imag[1] * temp_r + coeff_real[1] * temp_i;

      temp_r = ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + addrshift + 1 +
                                           HBE_ZERO_BAND_IDX][2 * max_n2];
      temp_i = ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + addrshift + 1 +
                                           HBE_ZERO_BAND_IDX][2 * max_n2 + 1];

      vec_y_r[0] += coeff_real[0] * temp_r - coeff_imag[0] * temp_i;
      vec_y_i[0] += coeff_imag[0] * temp_r + coeff_real[0] * temp_i;

 } else {
      WORD32 idx;
      d1 = 1.5;
      d2 = 0;
      mid_trans_fac = max_trans_fac;
      max_trans_fac = 3 - max_trans_fac;

      x_zero_band_r = qmf_in_buf_ri[2 * max_n2];
      x_zero_band_i = qmf_in_buf_ri[2 * max_n2 + 1];

      idx = (max_n1 & 3);
      idx = (idx + 1) & 3;
      coeff_real[0] = ixheaacd_hbe_post_anal_proc_interp_coeff[idx][0];
      coeff_imag[0] = ixheaacd_hbe_post_anal_proc_interp_coeff[idx][1];

      coeff_real[1] = coeff_real[0];
      coeff_imag[1] = -coeff_imag[0];

      vec_y_r[1] = qmf_in_buf_ri[2 * max_n1];
      vec_y_i[1] = qmf_in_buf_ri[2 * max_n1 + 1];

      addrshift = -2;

      temp_r = ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + addrshift +
                                           HBE_ZERO_BAND_IDX][2 * max_n1];
      temp_i = ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + addrshift +
                                           HBE_ZERO_BAND_IDX][2 * max_n1 + 1];

      vec_y_r[0] = coeff_real[1] * temp_r - coeff_imag[1] * temp_i;
      vec_y_i[0] = coeff_imag[1] * temp_r + coeff_real[1] * temp_i;

      temp_r = ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + addrshift + 1 +
                                           HBE_ZERO_BAND_IDX][2 * max_n1];
      temp_i = ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + addrshift + 1 +
                                           HBE_ZERO_BAND_IDX][2 * max_n1 + 1];

      vec_y_r[0] += coeff_real[0] * temp_r - coeff_imag[0] * temp_i;
      vec_y_i[0] += coeff_imag[0] * temp_r + coeff_real[0] * temp_i;
 }

    base = 1e-17;
    base = base + x_zero_band_r * x_zero_band_r;
    base = base + x_zero_band_i * x_zero_band_i;
    mag_scaling_fac = (FLOAT32)(ixheaacd_cbrt_calc((FLOAT32)base));
    x_zero_band_r *= mag_scaling_fac;
    x_zero_band_i *= mag_scaling_fac;
 for (k = 0; k < 2; k++) {
      base = 1e-17;
      base = base + vec_y_r[k] * vec_y_r[k];
      base = base + vec_y_i[k] * vec_y_i[k];
      mag_scaling_fac = (FLOAT32)(ixheaacd_cbrt_calc((FLOAT32)base));
      vec_y_r[k] *= mag_scaling_fac;
      vec_y_i[k] *= mag_scaling_fac;
 }

    temp_r = x_zero_band_r;
    temp_i = x_zero_band_i;
 for (idx = 0; idx < mid_trans_fac - 1; idx++) {
      FLOAT32 tmp = x_zero_band_r;
      x_zero_band_r = x_zero_band_r * temp_r - x_zero_band_i * temp_i;
      x_zero_band_i = tmp * temp_i + x_zero_band_i * temp_r;
 }

 for (k = 0; k < 2; k++) {
      temp_r = vec_y_r[k];
      temp_i = vec_y_i[k];
 for (idx = 0; idx < max_trans_fac - 1; idx++) {
        FLOAT32 tmp = vec_y_r[k];
        vec_y_r[k] = vec_y_r[k] * temp_r - vec_y_i[k] * temp_i;
        vec_y_i[k] = tmp * temp_i + vec_y_i[k] * temp_r;
 }
 }

 for (k = 0; k < 2; k++) {
      vec_o_r[k] = vec_y_r[k] * x_zero_band_r - vec_y_i[k] * x_zero_band_i;
      vec_o_i[k] = vec_y_r[k] * x_zero_band_i + vec_y_i[k] * x_zero_band_r;
 }

 {
      FLOAT32 cos_theta =
          ixheaacd_hbe_x_prod_cos_table_trans_3[(pitch_in_bins_idx << 1) + 0];
      FLOAT32 sin_theta =
          ixheaacd_hbe_x_prod_cos_table_trans_3[(pitch_in_bins_idx << 1) + 1];
 if (d2 < d1) {
        sin_theta = -sin_theta;
 }
      temp_r = vec_o_r[0];
      temp_i = vec_o_i[0];
      vec_o_r[0] = (FLOAT32)(cos_theta * temp_r - sin_theta * temp_i);
      vec_o_i[0] = (FLOAT32)(cos_theta * temp_i + sin_theta * temp_r);
 }

 for (k = 0; k < 2; k++) {
      ptr_hbe_txposer->qmf_out_buf[qmf_col_idx * 2 + (k + HBE_ZERO_BAND_IDX -
 1)][2 * qmf_band_idx] +=
 (FLOAT32)(mag_cmplx_gain * vec_o_r[k]);
      ptr_hbe_txposer
 ->qmf_out_buf[qmf_col_idx * 2 + (k + HBE_ZERO_BAND_IDX - 1)]
 [2 * qmf_band_idx + 1] +=
 (FLOAT32)(mag_cmplx_gain * vec_o_i[k]);
 }
 }
}
