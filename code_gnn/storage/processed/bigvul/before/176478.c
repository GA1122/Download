WORD32 ihevcd_free_dynamic_bufs(codec_t *ps_codec)
{

 if(ps_codec->pv_proc_jobq)
 {
        ihevcd_jobq_deinit(ps_codec->pv_proc_jobq);
        ps_codec->pv_proc_jobq = NULL;
 }

    ALIGNED_FREE(ps_codec, ps_codec->ps_tile);
    ALIGNED_FREE(ps_codec, ps_codec->pi4_entry_ofst);
    ALIGNED_FREE(ps_codec, ps_codec->s_parse.pu4_skip_cu_top);
    ALIGNED_FREE(ps_codec, ps_codec->s_parse.pu4_ct_depth_top);
    ALIGNED_FREE(ps_codec, ps_codec->pu1_pic_intra_flag);
    ALIGNED_FREE(ps_codec, ps_codec->pu1_pic_no_loop_filter_flag_base);
    ALIGNED_FREE(ps_codec, ps_codec->pv_proc_jobq_buf);
    ALIGNED_FREE(ps_codec, ps_codec->pu1_parse_map);
    ALIGNED_FREE(ps_codec, ps_codec->pu1_proc_map);
    ALIGNED_FREE(ps_codec, ps_codec->as_process[0].pu4_pic_pu_idx_left);
    ALIGNED_FREE(ps_codec, ps_codec->as_process[0].s_sao_ctxt.pu1_sao_src_left_luma);
    ALIGNED_FREE(ps_codec, ps_codec->as_process[0].s_bs_ctxt.pu4_pic_vert_bs);
    ALIGNED_FREE(ps_codec, ps_codec->pu1_tile_idx_base);
    ALIGNED_FREE(ps_codec, ps_codec->s_parse.ps_pic_sao);
    ALIGNED_FREE(ps_codec, ps_codec->pu1_bitsbuf_dynamic);
    ALIGNED_FREE(ps_codec, ps_codec->pv_tu_data);
    ALIGNED_FREE(ps_codec, ps_codec->pv_mv_bank_buf_base);
    ALIGNED_FREE(ps_codec, ps_codec->pu1_ref_pic_buf_base);
    ALIGNED_FREE(ps_codec, ps_codec->pu1_cur_chroma_ref_buf);

    ps_codec->u4_allocate_dynamic_done = 0;
 return IV_SUCCESS;
}
