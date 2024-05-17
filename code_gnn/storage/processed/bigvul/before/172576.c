void ih264d_form_pred_weight_matrix(dec_struct_t *ps_dec)
{
 dec_slice_params_t *ps_cur_slice;
    UWORD8 uc_num_ref_idx_l0_active, uc_num_ref_idx_l1_active;
    UWORD8 i, j;
    UWORD32 *pu4_mat_iwt_ofst;
    UWORD16 i2_idx;
    UWORD32 *pui32_weight_offset_l0, *pui32_weight_offset_l1;
    UWORD32 u4_temp;

    ps_cur_slice = ps_dec->ps_cur_slice;
    uc_num_ref_idx_l0_active = ps_cur_slice->u1_num_ref_idx_lx_active[0];
    uc_num_ref_idx_l1_active = ps_cur_slice->u1_num_ref_idx_lx_active[1];

    pu4_mat_iwt_ofst = ps_dec->pu4_wts_ofsts_mat;

 if(ps_cur_slice->u1_slice_type == B_SLICE)
 {
 for(i = 0; i < uc_num_ref_idx_l0_active; i++)
 {
            pui32_weight_offset_l0 = ps_cur_slice->u4_wt_ofst_lx[0][i];
 for(j = 0; j < uc_num_ref_idx_l1_active; j++)
 {
                pui32_weight_offset_l1 = ps_cur_slice->u4_wt_ofst_lx[1][j];
                i2_idx = i * uc_num_ref_idx_l0_active + j;
                i2_idx = X3(i2_idx);
  
                pu4_mat_iwt_ofst[0] = pui32_weight_offset_l0[0];
                pu4_mat_iwt_ofst[1] = pui32_weight_offset_l1[0];
                pu4_mat_iwt_ofst[2] = pui32_weight_offset_l0[1];
                pu4_mat_iwt_ofst[3] = pui32_weight_offset_l1[1];
                pu4_mat_iwt_ofst[4] = pui32_weight_offset_l0[2];
                pu4_mat_iwt_ofst[5] = pui32_weight_offset_l1[2];
                pu4_mat_iwt_ofst += 6;
 }
 }
 }
 else
 {
 for(i = 0; i < uc_num_ref_idx_l0_active; i++)
 {
            pui32_weight_offset_l0 = ps_cur_slice->u4_wt_ofst_lx[0][i];
            i2_idx = X3(i);
            u4_temp = (UWORD32)pui32_weight_offset_l0[0];
            pu4_mat_iwt_ofst[0] = u4_temp;
            u4_temp = (UWORD32)pui32_weight_offset_l0[1];
            pu4_mat_iwt_ofst[2] = u4_temp;
            u4_temp = (UWORD32)pui32_weight_offset_l0[2];
            pu4_mat_iwt_ofst[4] = u4_temp;
            pu4_mat_iwt_ofst += 6;
 }
 }
}
