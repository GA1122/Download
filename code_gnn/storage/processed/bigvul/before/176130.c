VOID ixheaacd_inv_emodulation(WORD32 *qmf_real,
                              ia_sbr_qmf_filter_bank_struct *syn_qmf,
                              ia_qmf_dec_tables_struct *qmf_dec_tables_ptr) {
  ixheaacd_cos_sin_mod(qmf_real, syn_qmf, (WORD16 *)qmf_dec_tables_ptr->w1024,
 (WORD32 *)qmf_dec_tables_ptr->dig_rev_table2_128);
}
