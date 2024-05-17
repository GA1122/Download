VOID ixheaacd_hbe_post_anal_xprod2(ia_esbr_hbe_txposer_struct *ptr_hbe_txposer,
                                   WORD32 qmf_voc_columns, WORD32 qmf_band_idx,
                                   FLOAT32 p, FLOAT32 *cos_sin_theta) {
  WORD32 i;
  FLOAT32 *norm_ptr = &ptr_hbe_txposer->norm_qmf_in_buf[1][2 * qmf_band_idx];
  FLOAT32 *out_ptr = &ptr_hbe_txposer->qmf_out_buf[1][2 * qmf_band_idx];
  FLOAT32 *x_norm_ptr =
 &ptr_hbe_txposer->norm_qmf_in_buf[HBE_ZERO_BAND_IDX][2 * qmf_band_idx];

  ixheaacd_norm_qmf_in_buf_2(ptr_hbe_txposer, qmf_band_idx);

 for (; qmf_band_idx < ptr_hbe_txposer->x_over_qmf[1]; qmf_band_idx++) {
    WORD32 n1, n2;
    FLOAT64 temp_fac;
    FLOAT32 mag_cmplx_gain = 1.666666667f;
    temp_fac = (2.0f * qmf_band_idx + 1 - p) * 0.5;
    n1 = ((WORD32)(temp_fac)) << 1;
    n2 = ((WORD32)(temp_fac + p)) << 1;

 for (i = 0; i < qmf_voc_columns; i++) {
      WORD32 k;
      FLOAT32 x_zero_band_r, x_zero_band_i;

      x_zero_band_r = *x_norm_ptr++;
      x_zero_band_i = *x_norm_ptr++;

 for (k = 1; k < (HBE_OPER_BLK_LEN_2 + 1); k++) {
 register FLOAT32 tmp_r, tmp_i;
        tmp_r = *norm_ptr++;
        tmp_i = *norm_ptr++;

 *out_ptr++ +=
 ((tmp_r * x_zero_band_r - tmp_i * x_zero_band_i) * 0.3333333f);

 *out_ptr++ +=
 ((tmp_r * x_zero_band_i + tmp_i * x_zero_band_r) * 0.3333333f);

        norm_ptr += 126;
        out_ptr += 126;
 }
      norm_ptr -= 128 * 9;
      out_ptr -= 128 * 8;
      x_norm_ptr += 126;

 {
        WORD32 max_trans_fac, max_n1, max_n2;
        FLOAT32 max_mag_value;
        FLOAT32 mag_zero_band, mag_n1_band, mag_n2_band, temp;

        FLOAT32 *qmf_in_buf_ri =
            ptr_hbe_txposer->qmf_in_buf[i + HBE_ZERO_BAND_IDX];

        mag_zero_band =
            qmf_in_buf_ri[2 * qmf_band_idx] * qmf_in_buf_ri[2 * qmf_band_idx] +
            qmf_in_buf_ri[2 * qmf_band_idx + 1] *
                qmf_in_buf_ri[2 * qmf_band_idx + 1];

        mag_n1_band = qmf_in_buf_ri[n1] * qmf_in_buf_ri[n1] +
                      qmf_in_buf_ri[n1 + 1] * qmf_in_buf_ri[n1 + 1];
        mag_n2_band = qmf_in_buf_ri[n2] * qmf_in_buf_ri[n2] +
                      qmf_in_buf_ri[n2 + 1] * qmf_in_buf_ri[n2 + 1];

        temp = min(mag_n1_band, mag_n2_band);

        max_mag_value = 0;
        max_trans_fac = 0;
        max_n1 = 0;
        max_n2 = 0;

 if (temp > 0) {
          max_mag_value = temp;
          max_trans_fac = 1;
          max_n1 = n1;
          max_n2 = n2;
 }

 if (max_mag_value > mag_zero_band && max_n1 >= 0 &&
            max_n2 < TWICE_QMF_SYNTH_CHANNELS_NUM) {
          FLOAT32 vec_y_r[2], vec_y_i[2];
          FLOAT32 temp_r, temp_i, tmp_r1;
          WORD32 mid_trans_fac, idx;
          FLOAT64 base;
          WORD32 k;
          FLOAT32 mag_scaling_fac = 0.0f;
          FLOAT32 x_zero_band_r = 0;
          FLOAT32 x_zero_band_i = 0;

          mid_trans_fac = 2 - max_trans_fac;

          x_zero_band_r = qmf_in_buf_ri[max_n1];
          x_zero_band_i = qmf_in_buf_ri[max_n1 + 1];
          base = 1e-17;
          base = base + x_zero_band_r * x_zero_band_r;
          base = base + x_zero_band_i * x_zero_band_i;

          mag_scaling_fac = (FLOAT32)(1.0f / base);
          mag_scaling_fac = (FLOAT32)sqrt(sqrt(mag_scaling_fac));

          x_zero_band_r *= mag_scaling_fac;
          x_zero_band_i *= mag_scaling_fac;

          temp_r = x_zero_band_r;
          temp_i = x_zero_band_i;
 for (idx = 0; idx < mid_trans_fac - 1; idx++) {
            FLOAT32 tmp = x_zero_band_r;
            x_zero_band_r = x_zero_band_r * temp_r - x_zero_band_i * temp_i;
            x_zero_band_i = tmp * temp_i + x_zero_band_i * temp_r;
 }

 for (k = 0; k < 2; k++) {
            temp_r = ptr_hbe_txposer
 ->qmf_in_buf[i + HBE_ZERO_BAND_IDX - 1 + k][max_n2];
            temp_i =
                ptr_hbe_txposer
 ->qmf_in_buf[i + HBE_ZERO_BAND_IDX - 1 + k][max_n2 + 1];

            base = 1e-17;
            base = base + temp_r * temp_r;
            base = base + temp_i * temp_i;

            mag_scaling_fac = (FLOAT32)(1.0f / base);
            mag_scaling_fac = (FLOAT32)sqrt(sqrt(mag_scaling_fac));

            temp_r *= mag_scaling_fac;
            temp_i *= mag_scaling_fac;

            vec_y_r[k] = temp_r;
            vec_y_i[k] = temp_i;
 }

          temp_r = vec_y_r[0] * x_zero_band_r - vec_y_i[0] * x_zero_band_i;
          temp_i = vec_y_r[0] * x_zero_band_i + vec_y_i[0] * x_zero_band_r;

          tmp_r1 =
 (FLOAT32)(cos_sin_theta[0] * temp_r - cos_sin_theta[1] * temp_i);
          temp_i =
 (FLOAT32)(cos_sin_theta[0] * temp_i + cos_sin_theta[1] * temp_r);

          ptr_hbe_txposer->qmf_out_buf[i * 2 + (HBE_ZERO_BAND_IDX - 1)]
 [2 * qmf_band_idx] +=
 (FLOAT32)(mag_cmplx_gain * tmp_r1);

          ptr_hbe_txposer->qmf_out_buf[i * 2 + (HBE_ZERO_BAND_IDX - 1)]
 [2 * qmf_band_idx + 1] +=
 (FLOAT32)(mag_cmplx_gain * temp_i);

          temp_r = vec_y_r[1] * x_zero_band_r - vec_y_i[1] * x_zero_band_i;
          temp_i = vec_y_r[1] * x_zero_band_i + vec_y_i[1] * x_zero_band_r;

          ptr_hbe_txposer->qmf_out_buf[i * 2 + (1 + HBE_ZERO_BAND_IDX - 1)]
 [2 * qmf_band_idx] +=
 (FLOAT32)(mag_cmplx_gain * temp_r);

          ptr_hbe_txposer->qmf_out_buf[i * 2 + (1 + HBE_ZERO_BAND_IDX - 1)]
 [2 * qmf_band_idx + 1] +=
 (FLOAT32)(mag_cmplx_gain * temp_i);
 }
 }
 }

    out_ptr -= (128 * 2 * qmf_voc_columns) - 2;
    norm_ptr -= (128 * qmf_voc_columns) - 2;
    x_norm_ptr -= (128 * qmf_voc_columns) - 2;
 }
}
