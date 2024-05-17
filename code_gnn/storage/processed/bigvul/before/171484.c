WORD32 ih264d_free_static_bufs(iv_obj_t *dec_hdl)
{
 dec_struct_t *ps_dec;

 void (*pf_aligned_free)(void *pv_mem_ctxt, void *pv_buf);
 void *pv_mem_ctxt;

    ps_dec = (dec_struct_t *)dec_hdl->pv_codec_handle;
    pf_aligned_free = ps_dec->pf_aligned_free;
    pv_mem_ctxt = ps_dec->pv_mem_ctxt;

    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_sps);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_pps);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pv_dec_thread_handle);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pv_bs_deblk_thread_handle);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_dpb_mgr);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_pred);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pv_disp_buf_mgr);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pv_pic_buf_mgr);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_pic_buf_base);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_dec_err_status);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_sei);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_dpb_cmds);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_bitstrm);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_cur_slice);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pv_scratch_sps_pps);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_bits_buf_static);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ppv_map_ref_idx_to_poc_base);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->p_cabac_ctxt_table_t);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_left_mb_ctxt_info);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_ref_buff_base);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pi2_pred1);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_temp_mc_buffer);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu1_init_dpb_base);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu4_mbaff_wt_mat);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pu4_wts_ofsts_mat);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_left_mvpred_addr);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->pv_mv_buf_mgr);
    PS_DEC_ALIGNED_FREE(ps_dec, ps_dec->ps_col_mv_base);
    PS_DEC_ALIGNED_FREE(ps_dec, dec_hdl->pv_codec_handle);

 if(dec_hdl)
 {
        pf_aligned_free(pv_mem_ctxt, dec_hdl);
 }
 return IV_SUCCESS;
}
