VOID ixheaacd_hbe_post_anal_prod4(ia_esbr_hbe_txposer_struct *ptr_hbe_txposer,
                                  WORD32 qmf_voc_columns, WORD32 qmf_band_idx) {
  WORD32 i, inp_band_idx;
  FLOAT32 *out_ptr = &ptr_hbe_txposer->qmf_out_buf[3][2 * qmf_band_idx];

  ixheaacd_norm_qmf_in_buf_4(ptr_hbe_txposer, ((qmf_band_idx >> 1) - 1));

 for (; qmf_band_idx < ptr_hbe_txposer->x_over_qmf[3]; qmf_band_idx++) {
    WORD32 ip_idx;
    FLOAT32 temp, temp_r, temp_i;
    FLOAT32 *norm_ptr, *x_norm_ptr;
    inp_band_idx = qmf_band_idx >> 1;
    ip_idx = (qmf_band_idx & 1) ? (inp_band_idx + 1) : (inp_band_idx - 1);

    norm_ptr = &ptr_hbe_txposer->norm_qmf_in_buf[0][2 * ip_idx];
    x_norm_ptr =
 &ptr_hbe_txposer->norm_qmf_in_buf[HBE_ZERO_BAND_IDX][2 * inp_band_idx];

 for (i = 0; i < qmf_voc_columns; i++) {
      WORD32 k;
      FLOAT32 x_zero_band_r, x_zero_band_i;

      temp_r = x_zero_band_r = *x_norm_ptr++;
      temp_i = x_zero_band_i = *x_norm_ptr++;

      temp = x_zero_band_r * x_zero_band_r - x_zero_band_i * x_zero_band_i;
      x_zero_band_i =
          x_zero_band_r * x_zero_band_i + x_zero_band_i * x_zero_band_r;

      x_zero_band_r = temp_r * temp - temp_i * x_zero_band_i;
      x_zero_band_i = temp_r * x_zero_band_i + temp_i * temp;

 for (k = 0; k < HBE_OPER_BLK_LEN_4; k++) {
        temp = *norm_ptr++;
        temp_i = *norm_ptr++;

        temp_r = temp * x_zero_band_r - temp_i * x_zero_band_i;
        temp_i = temp * x_zero_band_i + temp_i * x_zero_band_r;

 *out_ptr++ += (temp_r * 0.6666667f);
 *out_ptr++ += (temp_i * 0.6666667f);

        norm_ptr += 254;
        out_ptr += 126;
 }

      norm_ptr -= 128 * 11;
      out_ptr -= 128 * 4;
      x_norm_ptr += 126;
 }

    out_ptr -= (128 * 2 * qmf_voc_columns) - 2;
 }
}
