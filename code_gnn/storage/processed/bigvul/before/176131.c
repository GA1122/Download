VOID ixheaacd_inv_modulation_lp(WORD32 *qmf_real, WORD16 *filter_states,
                                ia_sbr_qmf_filter_bank_struct *syn_qmf,
                                ia_qmf_dec_tables_struct *qmf_dec_tables_ptr) {
  WORD32 L = syn_qmf->no_channels;
 const WORD32 M = (L >> 1);
  WORD32 *dct_in = qmf_real;
  WORD32 time_out[2 * NO_SYNTHESIS_CHANNELS];

  WORD32 ui_rem = ((WORD64)(&time_out[0]) % 8);
  WORD32 *ptime_out = (pVOID)((WORD8 *)&time_out[0] + 8 - ui_rem);

 if (L == 64)
    ixheaacd_dct2_64(dct_in, ptime_out, qmf_dec_tables_ptr, filter_states + M);
 else
    ixheaacd_dct2_32(dct_in, time_out, qmf_dec_tables_ptr, filter_states);

  filter_states[3 * M] = 0;
}
