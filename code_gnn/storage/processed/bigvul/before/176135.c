VOID ixheaacd_hbe_post_anal_prod2(ia_esbr_hbe_txposer_struct *ptr_hbe_txposer,
                                  WORD32 qmf_voc_columns, WORD32 qmf_band_idx) {
  WORD32 i;
  FLOAT32 *norm_ptr = &ptr_hbe_txposer->norm_qmf_in_buf[1][2 * qmf_band_idx];
  FLOAT32 *out_ptr = &ptr_hbe_txposer->qmf_out_buf[1][2 * qmf_band_idx];
  FLOAT32 *x_norm_ptr =
 &ptr_hbe_txposer->norm_qmf_in_buf[HBE_ZERO_BAND_IDX][2 * qmf_band_idx];

  ixheaacd_norm_qmf_in_buf_2(ptr_hbe_txposer, qmf_band_idx);

 for (; qmf_band_idx < ptr_hbe_txposer->x_over_qmf[1]; qmf_band_idx++) {
 for (i = 0; i < qmf_voc_columns; i++) {
      WORD32 k;
      FLOAT32 x_zero_band_r, x_zero_band_i;

      x_zero_band_r = *x_norm_ptr++;
      x_zero_band_i = *x_norm_ptr++;

 for (k = 0; k < HBE_OPER_BLK_LEN_2; k++) {
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
 }
    out_ptr -= (128 * 2 * qmf_voc_columns) - 2;
    norm_ptr -= (128 * qmf_voc_columns) - 2;
    x_norm_ptr -= (128 * qmf_voc_columns) - 2;
 }
}
