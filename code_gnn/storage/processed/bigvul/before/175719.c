WORD16 ih264d_free_dynamic_bufs(dec_struct_t * ps_dec)
{
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_bits_buf_dynamic);

    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_deblk_pic);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_dec_mb_map);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_recon_mb_map);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu2_slice_num_map);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_dec_slice_buf);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_frm_mb_info);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pi2_coeff_data);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_parse_mb_data);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_parse_part_params);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_deblk_top_mb);

 if(ps_dec->p_ctxt_inc_mb_map)
 {
        ps_dec->p_ctxt_inc_mb_map -= 1;
        PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->p_ctxt_inc_mb_map);
 }

    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_mv_p[0]);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_mv_p[1]);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_pred_pkd);
 {
        UWORD8 i;
 for(i = 0; i < MV_SCRATCH_BUFS; i++)
 {
            PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_mv_top_p[i]);
 }
 }

 if(ps_dec->pu1_y_intra_pred_line)
 {
        ps_dec->pu1_y_intra_pred_line -= MB_SIZE;
 }
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_y_intra_pred_line);

 if(ps_dec->pu1_u_intra_pred_line)
 {
        ps_dec->pu1_u_intra_pred_line -= MB_SIZE;
 }
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_u_intra_pred_line);

 if(ps_dec->pu1_v_intra_pred_line)
 {
        ps_dec->pu1_v_intra_pred_line -= MB_SIZE;
 }
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_v_intra_pred_line);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_nbr_mb_row);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_mv_bank_buf_base);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_pic_buf_base);
 return 0;
}
