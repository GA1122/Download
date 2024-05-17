WORD32 ihevcd_allocate_static_bufs(iv_obj_t **pps_codec_obj,
 ihevcd_cxa_create_ip_t *ps_create_ip,
 ihevcd_cxa_create_op_t *ps_create_op)
{
    WORD32 size;
 void *pv_buf;
    UWORD8 *pu1_buf;
    WORD32 i;
 codec_t *ps_codec;
    IV_API_CALL_STATUS_T status = IV_SUCCESS;
 void *(*pf_aligned_alloc)(void *pv_mem_ctxt, WORD32 alignment, WORD32 size);
 void (*pf_aligned_free)(void *pv_mem_ctxt, void *pv_buf);
 void *pv_mem_ctxt;

  
    ps_create_op->s_ivd_create_op_t.pv_handle = NULL;

    pf_aligned_alloc = ps_create_ip->s_ivd_create_ip_t.pf_aligned_alloc;
    pf_aligned_free = ps_create_ip->s_ivd_create_ip_t.pf_aligned_free;
    pv_mem_ctxt  = ps_create_ip->s_ivd_create_ip_t.pv_mem_ctxt;


    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, sizeof(iv_obj_t));
    RETURN_IF((NULL == pv_buf), IV_FAIL);
 *pps_codec_obj = (iv_obj_t *)pv_buf;
    ps_create_op->s_ivd_create_op_t.pv_handle = *pps_codec_obj;


 (*pps_codec_obj)->pv_codec_handle = NULL;
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, sizeof(codec_t));
    RETURN_IF((NULL == pv_buf), IV_FAIL);
 (*pps_codec_obj)->pv_codec_handle = (codec_t *)pv_buf;
    ps_codec = (codec_t *)pv_buf;

    memset(ps_codec, 0, sizeof(codec_t));

#ifndef LOGO_EN
    ps_codec->i4_share_disp_buf = ps_create_ip->s_ivd_create_ip_t.u4_share_disp_buf;
#else
    ps_codec->i4_share_disp_buf = 0;
#endif

  
 if((ps_create_ip->s_ivd_create_ip_t.e_output_format != IV_YUV_420P) &&
 (ps_create_ip->s_ivd_create_ip_t.e_output_format != IV_YUV_420SP_UV) &&
 (ps_create_ip->s_ivd_create_ip_t.e_output_format != IV_YUV_420SP_VU))
 {
        ps_codec->i4_share_disp_buf = 0;
 }

    ps_codec->e_chroma_fmt = ps_create_ip->s_ivd_create_ip_t.e_output_format;

    ps_codec->pf_aligned_alloc = pf_aligned_alloc;
    ps_codec->pf_aligned_free = pf_aligned_free;
    ps_codec->pv_mem_ctxt = pv_mem_ctxt;

  
    size = MAX_PROCESS_THREADS * ithread_get_handle_size();
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
        WORD32 handle_size = ithread_get_handle_size();
        ps_codec->apv_process_thread_handle[i] =
 (UWORD8 *)pv_buf + (i * handle_size);
 }

  
    size = MIN_BITSBUF_SIZE;
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size + 16);  
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_codec->pu1_bitsbuf_static = pv_buf;
    ps_codec->u4_bitsbuf_size_static = size;

  
    size = sizeof(buf_mgr_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_codec->pv_disp_buf_mgr = pv_buf;

  
    size = sizeof(dpb_mgr_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_codec->pv_dpb_mgr = pv_buf;

  
    size = sizeof(buf_mgr_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_codec->pv_pic_buf_mgr = pv_buf;

  
    size = sizeof(buf_mgr_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_codec->pv_mv_buf_mgr = pv_buf;

    size = MAX_VPS_CNT * sizeof(vps_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->ps_vps_base = pv_buf;
    ps_codec->s_parse.ps_vps_base = ps_codec->ps_vps_base;

    size = MAX_SPS_CNT * sizeof(sps_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->ps_sps_base = pv_buf;
    ps_codec->s_parse.ps_sps_base = ps_codec->ps_sps_base;

    size = MAX_PPS_CNT * sizeof(pps_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->ps_pps_base = pv_buf;
    ps_codec->s_parse.ps_pps_base = ps_codec->ps_pps_base;

    size = MAX_SLICE_HDR_CNT * sizeof(slice_header_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->ps_slice_hdr_base = (slice_header_t *)pv_buf;
    ps_codec->s_parse.ps_slice_hdr_base = ps_codec->ps_slice_hdr_base;


    SCALING_MAT_SIZE(size)
    size = (MAX_SPS_CNT + MAX_PPS_CNT) * size * sizeof(WORD16);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_codec->pi2_scaling_mat = (WORD16 *)pv_buf;


  
    size = BUF_MGR_MAX_CNT * sizeof(pic_buf_t);
    pv_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_codec->pv_pic_buf_base = (UWORD8 *)pv_buf;

  
    size = 4 * MAX_CTB_SIZE * MAX_CTB_SIZE;

  
    size *= 2;
    size *= MAX_PROCESS_THREADS;

    pu1_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pu1_buf), IV_FAIL);

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
        ps_codec->as_process[i].s_sao_ctxt.pu1_tmp_buf_luma = (UWORD8 *)pu1_buf;
        pu1_buf += 4 * MAX_CTB_SIZE * MAX_CTB_SIZE * sizeof(UWORD8);

        ps_codec->as_process[i].s_sao_ctxt.pu1_tmp_buf_chroma = (UWORD8 *)pu1_buf;
        pu1_buf += 4 * MAX_CTB_SIZE * MAX_CTB_SIZE * sizeof(UWORD8);
 }

  
  
  
    size = 3 * 16 * sizeof(UWORD8);
    pu1_buf = pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pu1_buf), IV_FAIL);
    memset(pu1_buf, 0, size);
    ps_codec->s_parse.pu1_luma_intra_pred_mode_left = pu1_buf;
    ps_codec->s_parse.pu1_luma_intra_pred_mode_top  = pu1_buf + 16;

 {
        WORD32 inter_pred_tmp_buf_size, ntaps_luma;
        WORD32 pic_pu_idx_map_size;

  
        ntaps_luma = 8;
        inter_pred_tmp_buf_size = sizeof(WORD16) * (MAX_CTB_SIZE + ntaps_luma) * MAX_CTB_SIZE;

        inter_pred_tmp_buf_size = ALIGN64(inter_pred_tmp_buf_size);

  
        pic_pu_idx_map_size = sizeof(WORD32) * (18 * 18);
        pic_pu_idx_map_size = ALIGN64(pic_pu_idx_map_size);

        size =  inter_pred_tmp_buf_size * 2;
        size += pic_pu_idx_map_size;
        size *= MAX_PROCESS_THREADS;

        pu1_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
        RETURN_IF((NULL == pu1_buf), IV_FAIL);
        memset(pu1_buf, 0, size);

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].pi2_inter_pred_tmp_buf1 = (WORD16 *)pu1_buf;
            pu1_buf += inter_pred_tmp_buf_size;

            ps_codec->as_process[i].pi2_inter_pred_tmp_buf2 = (WORD16 *)pu1_buf;
            pu1_buf += inter_pred_tmp_buf_size;

  
            ps_codec->as_process[i].pi2_itrans_intrmd_buf =
                            ps_codec->as_process[i].pi2_inter_pred_tmp_buf2;
            ps_codec->as_process[i].pi2_invscan_out =
                            ps_codec->as_process[i].pi2_inter_pred_tmp_buf1;

            ps_codec->as_process[i].pu4_pic_pu_idx_map = (UWORD32 *)pu1_buf;
            ps_codec->as_process[i].s_bs_ctxt.pu4_pic_pu_idx_map =
 (UWORD32 *)pu1_buf;
            pu1_buf += pic_pu_idx_map_size;


            ps_codec->as_process[i].i4_inter_pred_tmp_buf_strd = MAX_CTB_SIZE;

 }
 }
  
 {
 sps_t *ps_sps = ps_codec->ps_sps_base;
 pps_t *ps_pps = ps_codec->ps_pps_base;
        WORD16 *pi2_scaling_mat =  ps_codec->pi2_scaling_mat;
        WORD32 scaling_mat_size;

        SCALING_MAT_SIZE(scaling_mat_size);

 for(i = 0; i < MAX_SPS_CNT; i++)
 {
            ps_sps->pi2_scaling_mat  = pi2_scaling_mat;
            pi2_scaling_mat += scaling_mat_size;
            ps_sps++;
 }

 for(i = 0; i < MAX_PPS_CNT; i++)
 {
            ps_pps->pi2_scaling_mat  = pi2_scaling_mat;
            pi2_scaling_mat += scaling_mat_size;
            ps_pps++;
 }
 }

 return (status);
}
