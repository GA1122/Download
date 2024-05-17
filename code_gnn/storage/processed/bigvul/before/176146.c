WORD32 ixheaacd_qmf_hbe_apply(ia_esbr_hbe_txposer_struct *ptr_hbe_txposer,
                              FLOAT32 qmf_buf_real[][64],
                              FLOAT32 qmf_buf_imag[][64], WORD32 num_columns,
                              FLOAT32 pv_qmf_buf_real[][64],
                              FLOAT32 pv_qmf_buf_imag[][64],
                              WORD32 pitch_in_bins) {
  WORD32 i, qmf_band_idx;
  WORD32 qmf_voc_columns = ptr_hbe_txposer->no_bins / 2;
  WORD32 err_code = 0;

  memcpy(ptr_hbe_txposer->ptr_input_buf,
         ptr_hbe_txposer->ptr_input_buf +
             ptr_hbe_txposer->no_bins * ptr_hbe_txposer->synth_size,
         ptr_hbe_txposer->synth_size * sizeof(FLOAT32));

  ixheaacd_real_synth_filt(ptr_hbe_txposer, num_columns, qmf_buf_real,
                           qmf_buf_imag);

 for (i = 0; i < HBE_OPER_WIN_LEN - 1; i++) {
    memcpy(ptr_hbe_txposer->qmf_in_buf[i],
           ptr_hbe_txposer->qmf_in_buf[i + qmf_voc_columns],
           TWICE_QMF_SYNTH_CHANNELS_NUM * sizeof(FLOAT32));
 }

  err_code = ixheaacd_complex_anal_filt(ptr_hbe_txposer);
 if (err_code) return err_code;

 for (i = 0; i < (ptr_hbe_txposer->hbe_qmf_out_len - ptr_hbe_txposer->no_bins);
       i++) {
    memcpy(ptr_hbe_txposer->qmf_out_buf[i],
           ptr_hbe_txposer->qmf_out_buf[i + ptr_hbe_txposer->no_bins],
           TWICE_QMF_SYNTH_CHANNELS_NUM * sizeof(FLOAT32));
 }

 for (; i < ptr_hbe_txposer->hbe_qmf_out_len; i++) {
    memset(ptr_hbe_txposer->qmf_out_buf[i], 0,
           TWICE_QMF_SYNTH_CHANNELS_NUM * sizeof(FLOAT32));
 }

  ixheaacd_hbe_post_anal_process(ptr_hbe_txposer, pitch_in_bins,
                                 ptr_hbe_txposer->upsamp_4_flag);

 for (i = 0; i < ptr_hbe_txposer->no_bins; i++) {
 for (qmf_band_idx = ptr_hbe_txposer->start_band;
         qmf_band_idx < ptr_hbe_txposer->end_band; qmf_band_idx++) {
      pv_qmf_buf_real[i][qmf_band_idx] =
 (FLOAT32)(ptr_hbe_txposer->qmf_out_buf[i][2 * qmf_band_idx] *
                        ixheaacd_phase_vocoder_cos_table[qmf_band_idx] -
                    ptr_hbe_txposer->qmf_out_buf[i][2 * qmf_band_idx + 1] *
                        ixheaacd_phase_vocoder_sin_table[qmf_band_idx]);

      pv_qmf_buf_imag[i][qmf_band_idx] =
 (FLOAT32)(ptr_hbe_txposer->qmf_out_buf[i][2 * qmf_band_idx] *
                        ixheaacd_phase_vocoder_sin_table[qmf_band_idx] +
                    ptr_hbe_txposer->qmf_out_buf[i][2 * qmf_band_idx + 1] *
                        ixheaacd_phase_vocoder_cos_table[qmf_band_idx]);
 }
 }
 return 0;
}
