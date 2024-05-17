WORD32 ihevcd_free_static_bufs(iv_obj_t *ps_codec_obj)
{
 codec_t *ps_codec;

 void (*pf_aligned_free)(void *pv_mem_ctxt, void *pv_buf);
 void *pv_mem_ctxt;

    ps_codec = (codec_t *)ps_codec_obj->pv_codec_handle;
    pf_aligned_free = ps_codec->pf_aligned_free;
    pv_mem_ctxt = ps_codec->pv_mem_ctxt;


    ALIGNED_FREE(ps_codec, ps_codec->apv_process_thread_handle[0]);
    ALIGNED_FREE(ps_codec, ps_codec->pu1_bitsbuf_static);

    ALIGNED_FREE(ps_codec, ps_codec->pv_disp_buf_mgr);
    ALIGNED_FREE(ps_codec, ps_codec->pv_dpb_mgr);
    ALIGNED_FREE(ps_codec, ps_codec->pv_pic_buf_mgr);
    ALIGNED_FREE(ps_codec, ps_codec->pv_mv_buf_mgr);
    ALIGNED_FREE(ps_codec, ps_codec->ps_vps_base);
    ALIGNED_FREE(ps_codec, ps_codec->ps_sps_base);
    ALIGNED_FREE(ps_codec, ps_codec->ps_pps_base);
    ALIGNED_FREE(ps_codec, ps_codec->ps_slice_hdr_base);
    ALIGNED_FREE(ps_codec, ps_codec->pi2_scaling_mat);
    ALIGNED_FREE(ps_codec, ps_codec->pv_pic_buf_base);
    ALIGNED_FREE(ps_codec, ps_codec->s_parse.pu1_luma_intra_pred_mode_left);
    ALIGNED_FREE(ps_codec, ps_codec->as_process[0].s_sao_ctxt.pu1_tmp_buf_luma);
    ALIGNED_FREE(ps_codec, ps_codec->as_process[0].pi2_inter_pred_tmp_buf1);
    ALIGNED_FREE(ps_codec, ps_codec_obj->pv_codec_handle);

 if(ps_codec_obj)
 {
        pf_aligned_free(pv_mem_ctxt, ps_codec_obj);
 }

 return IV_SUCCESS;

}
