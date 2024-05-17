VOID ixheaacd_hbe_post_anal_process(ia_esbr_hbe_txposer_struct *ptr_hbe_txposer,
                                    WORD32 pitch_in_bins,
                                    WORD32 sbr_upsamp_4_flg) {
  FLOAT32 p;
  WORD32 trans_fac;
  WORD32 qmf_voc_columns = ptr_hbe_txposer->no_bins / 2;
  FLOAT32 cos_sin_theta[2];

  p = (sbr_upsamp_4_flg) ? (FLOAT32)(pitch_in_bins * 0.04166666666666)
 : (FLOAT32)(pitch_in_bins * 0.08333333333333);

 if (p < SBR_CONST_PMIN) {
    trans_fac = 2;
 if (trans_fac <= ptr_hbe_txposer->max_stretch)
      ixheaacd_hbe_post_anal_prod2(ptr_hbe_txposer, qmf_voc_columns,
                                   ptr_hbe_txposer->x_over_qmf[0]);

    trans_fac = 3;
 if (trans_fac <= ptr_hbe_txposer->max_stretch)
      ixheaacd_hbe_post_anal_prod3(ptr_hbe_txposer, qmf_voc_columns,
                                   ptr_hbe_txposer->x_over_qmf[1]);

    trans_fac = 4;
 if (trans_fac <= ptr_hbe_txposer->max_stretch)
      ixheaacd_hbe_post_anal_prod4(ptr_hbe_txposer, qmf_voc_columns,
                                   ptr_hbe_txposer->x_over_qmf[2]);

 } else {
    trans_fac = 2;
 if (trans_fac <= ptr_hbe_txposer->max_stretch) {
      cos_sin_theta[0] = ixheaacd_hbe_x_prod_cos_table_trans_2
 [((pitch_in_bins + sbr_upsamp_4_flg * 128) << 1) + 0];
      cos_sin_theta[1] = ixheaacd_hbe_x_prod_cos_table_trans_2
 [((pitch_in_bins + sbr_upsamp_4_flg * 128) << 1) + 1];

      ixheaacd_hbe_post_anal_xprod2(ptr_hbe_txposer, qmf_voc_columns,
                                    ptr_hbe_txposer->x_over_qmf[0], p,
                                    cos_sin_theta);
 }

    trans_fac = 3;
 if (trans_fac <= ptr_hbe_txposer->max_stretch)
      ixheaacd_hbe_post_anal_xprod3(ptr_hbe_txposer, qmf_voc_columns,
                                    ptr_hbe_txposer->x_over_qmf[1], p,
 (pitch_in_bins + sbr_upsamp_4_flg * 128));

    trans_fac = 4;
 if (trans_fac <= ptr_hbe_txposer->max_stretch)
      ixheaacd_hbe_post_anal_xprod4(ptr_hbe_txposer, qmf_voc_columns,
                                    ptr_hbe_txposer->x_over_qmf[2], p,
 (pitch_in_bins + sbr_upsamp_4_flg * 128));
 }
}
