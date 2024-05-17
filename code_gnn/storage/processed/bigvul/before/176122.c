VOID ixheaacd_cplx_anal_qmffilt(const WORD16 *time_sample_buf,
                                ia_sbr_scale_fact_struct *sbr_scale_factor,
                                WORD32 **qmf_real, WORD32 **qmf_imag,
                                ia_sbr_qmf_filter_bank_struct *qmf_bank,
                                ia_qmf_dec_tables_struct *qmf_dec_tables_ptr,
                                WORD32 ch_fac, WORD32 low_pow_flag,
                                WORD audio_object_type) {
  WORD32 i, k;
  WORD32 num_time_slots = qmf_bank->num_time_slots;

  WORD32 analysis_buffer[4 * NO_ANALYSIS_CHANNELS];
  WORD16 *filter_states = qmf_bank->core_samples_buffer;

  WORD16 *fp1, *fp2, *tmp;

  WORD16 *filter_1;
  WORD16 *filter_2;
  WORD16 *filt_ptr;
 if (audio_object_type != AOT_ER_AAC_ELD &&
      audio_object_type != AOT_ER_AAC_LD) {
    qmf_bank->filter_pos +=
 (qmf_dec_tables_ptr->qmf_c - qmf_bank->analy_win_coeff);
    qmf_bank->analy_win_coeff = qmf_dec_tables_ptr->qmf_c;
 } else {
    qmf_bank->filter_pos +=
 (qmf_dec_tables_ptr->qmf_c_eld3 - qmf_bank->analy_win_coeff);
    qmf_bank->analy_win_coeff = qmf_dec_tables_ptr->qmf_c_eld3;
 }

  filter_1 = qmf_bank->filter_pos;

 if (audio_object_type != AOT_ER_AAC_ELD &&
      audio_object_type != AOT_ER_AAC_LD) {
    filter_2 = filter_1 + 64;
 } else {
    filter_2 = filter_1 + 32;
 }

  sbr_scale_factor->st_lb_scale = 0;
  sbr_scale_factor->lb_scale = -10;
 if (!low_pow_flag) {
 if (audio_object_type != AOT_ER_AAC_ELD &&
        audio_object_type != AOT_ER_AAC_LD) {
      sbr_scale_factor->lb_scale = -8;
 } else {
      sbr_scale_factor->lb_scale = -9;
 }
    qmf_bank->cos_twiddle =
 (WORD16 *)qmf_dec_tables_ptr->sbr_sin_cos_twiddle_l32;
    qmf_bank->alt_sin_twiddle =
 (WORD16 *)qmf_dec_tables_ptr->sbr_alt_sin_twiddle_l32;
 if (audio_object_type != AOT_ER_AAC_ELD &&
        audio_object_type != AOT_ER_AAC_LD) {
      qmf_bank->t_cos = (WORD16 *)qmf_dec_tables_ptr->sbr_t_cos_sin_l32;
 } else {
      qmf_bank->t_cos =
 (WORD16 *)qmf_dec_tables_ptr->ixheaacd_sbr_t_cos_sin_l32_eld;
 }
 }

  fp1 = qmf_bank->anal_filter_states;
  fp2 = qmf_bank->anal_filter_states + NO_ANALYSIS_CHANNELS;

 if (audio_object_type == AOT_ER_AAC_ELD ||
      audio_object_type == AOT_ER_AAC_LD) {
    filter_2 = qmf_bank->filter_2;
    fp1 = qmf_bank->fp1_anal;
    fp2 = qmf_bank->fp2_anal;
 }

 for (i = 0; i < num_time_slots; i++) {
 for (k = 0; k < NO_ANALYSIS_CHANNELS; k++)
      filter_states[NO_ANALYSIS_CHANNELS - 1 - k] = time_sample_buf[ch_fac * k];

 if (audio_object_type != AOT_ER_AAC_ELD &&
        audio_object_type != AOT_ER_AAC_LD) {
      ixheaacd_sbr_qmfanal32_winadds(fp1, fp2, filter_1, filter_2,
                                     analysis_buffer, filter_states,
                                     time_sample_buf, ch_fac);
 }

 else {
      ixheaacd_sbr_qmfanal32_winadd_eld(fp1, fp2, filter_1, filter_2,
                                        analysis_buffer);
 }

    time_sample_buf += NO_ANALYSIS_CHANNELS * ch_fac;

    filter_states -= NO_ANALYSIS_CHANNELS;
 if (filter_states < qmf_bank->anal_filter_states) {
      filter_states = qmf_bank->anal_filter_states + 288;
 }

    tmp = fp1;
    fp1 = fp2;
    fp2 = tmp;
 if (audio_object_type != AOT_ER_AAC_ELD &&
        audio_object_type != AOT_ER_AAC_LD) {
      filter_1 += 64;
      filter_2 += 64;
 } else {
      filter_1 += 32;
      filter_2 += 32;
 }

    filt_ptr = filter_1;
    filter_1 = filter_2;
    filter_2 = filt_ptr;
 if (audio_object_type != AOT_ER_AAC_ELD &&
        audio_object_type != AOT_ER_AAC_LD) {
 if (filter_2 > (qmf_bank->analy_win_coeff + 640)) {
        filter_1 = (WORD16 *)qmf_bank->analy_win_coeff;
        filter_2 = (WORD16 *)qmf_bank->analy_win_coeff + 64;
 }
 } else {
 if (filter_2 > (qmf_bank->analy_win_coeff + 320)) {
        filter_1 = (WORD16 *)qmf_bank->analy_win_coeff;
        filter_2 = (WORD16 *)qmf_bank->analy_win_coeff + 32;
 }
 }

 if (!low_pow_flag) {
      ixheaacd_fwd_modulation(analysis_buffer, qmf_real[i], qmf_imag[i],
                              qmf_bank, qmf_dec_tables_ptr);
 } else {
      ixheaacd_dct3_32(
 (WORD32 *)analysis_buffer, qmf_real[i], qmf_dec_tables_ptr->dct23_tw,
          qmf_dec_tables_ptr->post_fft_tbl, qmf_dec_tables_ptr->w_16,
          qmf_dec_tables_ptr->dig_rev_table4_16);
 }
 }

  qmf_bank->filter_pos = filter_1;
  qmf_bank->core_samples_buffer = filter_states;

 if (audio_object_type == AOT_ER_AAC_ELD || audio_object_type == AOT_ER_AAC_LD)

 {
    qmf_bank->fp1_anal = fp1;
    qmf_bank->fp2_anal = fp2;
    qmf_bank->filter_2 = filter_2;
 }
}
