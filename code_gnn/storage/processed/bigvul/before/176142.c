VOID ixheaacd_hbe_xprod_proc_4(ia_esbr_hbe_txposer_struct *ptr_hbe_txposer,
                               WORD32 qmf_band_idx, WORD32 qmf_col_idx,
                               FLOAT32 p, WORD32 pitch_in_bins_idx) {
  WORD32 k;
  WORD32 inp_band_idx = qmf_band_idx >> 1;
  WORD32 tr, n1, n2, max_trans_fac, max_n1, max_n2;

  FLOAT64 temp_fac;
  FLOAT32 max_mag_value, mag_zero_band, mag_n1_band, mag_n2_band, temp;
  FLOAT32 temp_r, temp_i;
  FLOAT32 mag_cmplx_gain = 2.0f;

  FLOAT32 *qmf_in_buf_ri =
      ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + HBE_ZERO_BAND_IDX];

  mag_zero_band =
      qmf_in_buf_ri[2 * inp_band_idx] * qmf_in_buf_ri[2 * inp_band_idx] +
      qmf_in_buf_ri[2 * inp_band_idx + 1] * qmf_in_buf_ri[2 * inp_band_idx + 1];

  max_mag_value = 0;
  max_n1 = max_n2 = max_trans_fac = 0;

 for (tr = 1; tr < 4; tr++) {
    temp_fac = (2.0f * qmf_band_idx + 1 - tr * p) * 0.25;
    n1 = ((WORD32)(temp_fac)) << 1;
    n2 = ((WORD32)(temp_fac + p)) << 1;

    mag_n1_band = qmf_in_buf_ri[n1] * qmf_in_buf_ri[n1] +
                  qmf_in_buf_ri[n1 + 1] * qmf_in_buf_ri[n1 + 1];
    mag_n2_band = qmf_in_buf_ri[n2] * qmf_in_buf_ri[n2] +
                  qmf_in_buf_ri[n2 + 1] * qmf_in_buf_ri[n2 + 1];

    temp = min(mag_n1_band, mag_n2_band);

 if (temp > max_mag_value) {
      max_mag_value = temp;
      max_trans_fac = tr;
      max_n1 = n1;
      max_n2 = n2;
 }
 }
 if (max_mag_value > mag_zero_band && max_n1 >= 0 &&
      max_n2 < TWICE_QMF_SYNTH_CHANNELS_NUM) {
    FLOAT32 vec_y_r[2], vec_y_i[2], vec_o_r[2], vec_o_i[2];
    FLOAT32 d1, d2;
    WORD32 mid_trans_fac, idx;
    FLOAT32 x_zero_band_r;
    FLOAT32 x_zero_band_i;
    FLOAT64 base = 1e-17;
    FLOAT32 mag_scaling_fac = 0.0f;

    x_zero_band_r = 0;
    x_zero_band_i = 0;
    mid_trans_fac = 4 - max_trans_fac;

 if (max_trans_fac == 1) {
      d1 = 0;
      d2 = 2;
      x_zero_band_r = qmf_in_buf_ri[max_n1];
      x_zero_band_i = qmf_in_buf_ri[max_n1 + 1];
 for (k = 0; k < 2; k++) {
        vec_y_r[k] =
            ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + HBE_ZERO_BAND_IDX +
 2 * (k - 1)][max_n2];
        vec_y_i[k] =
            ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + HBE_ZERO_BAND_IDX +
 2 * (k - 1)][max_n2 + 1];
 }
 } else if (max_trans_fac == 2) {
      d1 = 0;
      d2 = 1;
      x_zero_band_r = qmf_in_buf_ri[max_n1];
      x_zero_band_i = qmf_in_buf_ri[max_n1 + 1];
 for (k = 0; k < 2; k++) {
        vec_y_r[k] =
            ptr_hbe_txposer
 ->qmf_in_buf[qmf_col_idx + HBE_ZERO_BAND_IDX + (k - 1)][max_n2];
        vec_y_i[k] =
            ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + HBE_ZERO_BAND_IDX +
 (k - 1)][max_n2 + 1];
 }
 } else {
      d1 = 2;
      d2 = 0;
      mid_trans_fac = max_trans_fac;
      max_trans_fac = 4 - max_trans_fac;
      x_zero_band_r = qmf_in_buf_ri[max_n2];
      x_zero_band_i = qmf_in_buf_ri[max_n2 + 1];
 for (k = 0; k < 2; k++) {
        vec_y_r[k] =
            ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + HBE_ZERO_BAND_IDX +
 2 * (k - 1)][max_n1];
        vec_y_i[k] =
            ptr_hbe_txposer->qmf_in_buf[qmf_col_idx + HBE_ZERO_BAND_IDX +
 2 * (k - 1)][max_n1 + 1];
 }
 }

    base = 1e-17;
    base = base + x_zero_band_r * x_zero_band_r;
    base = base + x_zero_band_i * x_zero_band_i;
 {
      temp = (FLOAT32)sqrt(sqrt(base));
      mag_scaling_fac = temp * (FLOAT32)(sqrt(temp));
      mag_scaling_fac = 1 / mag_scaling_fac;
 }

    x_zero_band_r *= mag_scaling_fac;
    x_zero_band_i *= mag_scaling_fac;
 for (k = 0; k < 2; k++) {
      base = 1e-17;
      base = base + vec_y_r[k] * vec_y_r[k];
      base = base + vec_y_i[k] * vec_y_i[k];
 {
        temp = (FLOAT32)sqrt(sqrt(base));
        mag_scaling_fac = temp * (FLOAT32)(sqrt(temp));

        mag_scaling_fac = 1 / mag_scaling_fac;
 }
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
      FLOAT32 cos_theta;
      FLOAT32 sin_theta;

 if (d2 == 1) {
        cos_theta =
            ixheaacd_hbe_x_prod_cos_table_trans_4_1[(pitch_in_bins_idx << 1) +
 0];
        sin_theta =
            ixheaacd_hbe_x_prod_cos_table_trans_4_1[(pitch_in_bins_idx << 1) +
 1];
 } else {
        cos_theta =
            ixheaacd_hbe_x_prod_cos_table_trans_4[(pitch_in_bins_idx << 1) + 0];
        sin_theta =
            ixheaacd_hbe_x_prod_cos_table_trans_4[(pitch_in_bins_idx << 1) + 1];
 if (d2 < d1) {
          sin_theta = -sin_theta;
 }
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
