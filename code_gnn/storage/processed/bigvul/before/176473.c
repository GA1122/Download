WORD32 ihevcd_allocate_dynamic_bufs(codec_t *ps_codec)
{
    WORD32 max_tile_cols, max_tile_rows;
    WORD32 max_ctb_rows, max_ctb_cols;
    WORD32 max_num_cu_cols;
    WORD32 max_num_cu_rows;
    WORD32 max_num_4x4_cols;
    WORD32 max_ctb_cnt;
    WORD32 wd;
    WORD32 ht;
    WORD32 i;
    WORD32 max_dpb_size;
 void *pv_mem_ctxt = ps_codec->pv_mem_ctxt;
 void *pv_buf;
    UWORD8 *pu1_buf;
    WORD32 size;

    wd = ALIGN64(ps_codec->i4_wd);
    ht = ALIGN64(ps_codec->i4_ht);

    max_tile_cols = (wd + MIN_TILE_WD - 1) / MIN_TILE_WD;
    max_tile_rows = (ht + MIN_TILE_HT - 1) / MIN_TILE_HT;
    max_ctb_rows  = ht / MIN_CTB_SIZE;
    max_ctb_cols  = wd / MIN_CTB_SIZE;
    max_ctb_cnt   = max_ctb_rows * max_ctb_cols;
    max_num_cu_cols = wd / MIN_CU_SIZE;
    max_num_cu_rows = ht / MIN_CU_SIZE;
    max_num_4x4_cols = wd / 4;

  
    size = max_tile_cols * max_tile_rows;
    size *= sizeof(tile_t);
    size *= MAX_PPS_CNT;

    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->ps_tile = (tile_t *)pv_buf;


  
  
    size = max_tile_cols * max_tile_rows;

  
  
  
  
  
  
    size  += max_ctb_rows;
    size *= sizeof(WORD32);

    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->pi4_entry_ofst = (WORD32 *)pv_buf;

  
  
    size = max_num_cu_cols / 8;
    size = ALIGN4(size);
    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->s_parse.pu4_skip_cu_top = (UWORD32 *)pv_buf;

  
  
    size =  max_num_cu_cols / 4;
    size = ALIGN4(size);
    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->s_parse.pu4_ct_depth_top = (UWORD32 *)pv_buf;

  
  
    size =  max_num_cu_cols * max_num_cu_rows / 8;
    size = ALIGN4(size);
    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->pu1_pic_intra_flag = (UWORD8 *)pv_buf;
    ps_codec->s_parse.pu1_pic_intra_flag = ps_codec->pu1_pic_intra_flag;

  
  
  
    size = ((max_num_cu_cols + 8) * (max_num_cu_rows + 8)) / 8;
    size = ALIGN4(size);
    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 1, size);
 {
        WORD32 loop_filter_strd = (wd + 63) >> 6;
        ps_codec->pu1_pic_no_loop_filter_flag_base = pv_buf;
  
        ps_codec->pu1_pic_no_loop_filter_flag = (UWORD8 *)pv_buf + loop_filter_strd + 1;
        ps_codec->s_parse.pu1_pic_no_loop_filter_flag = ps_codec->pu1_pic_no_loop_filter_flag;
        ps_codec->s_parse.s_deblk_ctxt.pu1_pic_no_loop_filter_flag = ps_codec->pu1_pic_no_loop_filter_flag;
        ps_codec->s_parse.s_sao_ctxt.pu1_pic_no_loop_filter_flag = ps_codec->pu1_pic_no_loop_filter_flag;
 }

  
 {
 pps_t *ps_pps = ps_codec->ps_pps_base;
 tile_t *ps_tile =  ps_codec->ps_tile;

 for(i = 0; i < MAX_PPS_CNT; i++)
 {
            ps_pps->ps_tile = ps_tile;
            ps_tile += (max_tile_cols * max_tile_rows);
            ps_pps++;
 }

 }

  

  
    size  = max_ctb_rows;

  
    size  *= max_tile_cols;

  
    size  += max_ctb_rows;

    size *= sizeof(proc_job_t);

    size += ihevcd_jobq_ctxt_size();
    size = ALIGN4(size);

    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    ps_codec->pv_proc_jobq_buf = pv_buf;
    ps_codec->i4_proc_jobq_buf_size = size;

    size =  max_ctb_cnt;
    size = ALIGN4(size);
    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->pu1_parse_map = (UWORD8 *)pv_buf;

    size =  max_ctb_cnt;
    size = ALIGN4(size);
    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->pu1_proc_map = (UWORD8 *)pv_buf;

  
  
    size = (max_num_4x4_cols    + MAX_PROCESS_THREADS * (MAX_CTB_SIZE / 4)  + 1 ) * sizeof(WORD32);
    size = ALIGN4(size);
    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
        UWORD32 *pu4_buf = (UWORD32 *)pv_buf;
        ps_codec->as_process[i].pu4_pic_pu_idx_left = pu4_buf + i * (MAX_CTB_SIZE / 4);
        memset(ps_codec->as_process[i].pu4_pic_pu_idx_left, 0, sizeof(UWORD32) * MAX_CTB_SIZE / 4);
        ps_codec->as_process[i].pu4_pic_pu_idx_top = pu4_buf + MAX_PROCESS_THREADS * (MAX_CTB_SIZE / 4);
 }
    memset(ps_codec->as_process[0].pu4_pic_pu_idx_top, 0, sizeof(UWORD32) * (wd / 4 + 1));

 {
  
        size  = sizeof(UWORD8) * (MAX(ht, wd));

  
        size += sizeof(UWORD8) * (MAX(ht, wd));

  
        size += sizeof(UWORD8) * wd;

  
        size += sizeof(UWORD8) * wd;

  
        size += sizeof(UWORD8) * max_ctb_rows;

  
        size += sizeof(UWORD8) * max_ctb_rows * 2;

  
        size += sizeof(UWORD8) * max_ctb_rows;

  
        size += sizeof(UWORD8) * max_ctb_rows * 2;

  
        size += sizeof(UWORD8) * max_ctb_cols;

  
        size += sizeof(UWORD8) * max_ctb_cols * 2;

  
        size += sizeof(UWORD8) * max_ctb_rows;

  
        size += sizeof(UWORD8) * max_ctb_rows * 2;
        size = ALIGN64(size);

        pu1_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
        RETURN_IF((NULL == pu1_buf), IV_FAIL);
        memset(pu1_buf, 0, size);

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_left_luma = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_left_luma = (UWORD8 *)pu1_buf;
        pu1_buf += MAX(ht, wd);

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_left_chroma = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_left_chroma = (UWORD8 *)pu1_buf;
        pu1_buf += MAX(ht, wd);
 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_top_luma = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_top_luma = (UWORD8 *)pu1_buf;
        pu1_buf += wd;

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_top_chroma = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_top_chroma = (UWORD8 *)pu1_buf;
        pu1_buf += wd;
 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_luma_top_left_ctb = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_luma_top_left_ctb = (UWORD8 *)pu1_buf;
        pu1_buf += ht / MIN_CTB_SIZE;

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_chroma_top_left_ctb = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_chroma_top_left_ctb = (UWORD8 *)pu1_buf;
        pu1_buf += (ht / MIN_CTB_SIZE) * 2;

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_top_left_luma_curr_ctb = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_top_left_luma_curr_ctb = (UWORD8 *)pu1_buf;
        pu1_buf += ht / MIN_CTB_SIZE;

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_top_left_chroma_curr_ctb = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_top_left_chroma_curr_ctb = (UWORD8 *)pu1_buf;

        pu1_buf += (ht / MIN_CTB_SIZE) * 2;
 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_top_left_luma_top_right = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_top_left_luma_top_right = (UWORD8 *)pu1_buf;

        pu1_buf += wd / MIN_CTB_SIZE;
 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_top_left_chroma_top_right = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_top_left_chroma_top_right = (UWORD8 *)pu1_buf;

        pu1_buf += (wd / MIN_CTB_SIZE) * 2;

  
 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_top_left_luma_bot_left = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_top_left_luma_bot_left = (UWORD8 *)pu1_buf;

        pu1_buf += (ht / MIN_CTB_SIZE);

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_sao_ctxt.pu1_sao_src_top_left_chroma_bot_left = (UWORD8 *)pu1_buf;
 }
        ps_codec->s_parse.s_sao_ctxt.pu1_sao_src_top_left_chroma_bot_left = (UWORD8 *)pu1_buf;

        pu1_buf += (ht / MIN_CTB_SIZE) * 2;
 }


 {
        UWORD8 *pu1_buf = (UWORD8 *)pv_buf;
        WORD32 vert_bs_size, horz_bs_size;
        WORD32 qp_const_flag_size;
        WORD32 qp_size;
        WORD32 num_8x8;

  
        vert_bs_size = wd / 8 + 2 * MAX_CTB_SIZE / 8;

  
        vert_bs_size *= (ht + MAX_CTB_SIZE) / MIN_TU_SIZE;

  
        vert_bs_size /= 8;

  
        vert_bs_size *= 2;

  
        horz_bs_size = ht / 8 + MAX_CTB_SIZE / 8;

  
        horz_bs_size *= (wd + MAX_CTB_SIZE) / MIN_TU_SIZE;

  
        horz_bs_size /= 8;

  
        horz_bs_size *= 2;

  
        qp_const_flag_size = wd / MIN_CTB_SIZE + 1  ;

  
        qp_const_flag_size *= ht / MIN_CTB_SIZE;

  
        qp_const_flag_size /= 8;

  
        num_8x8 = (ht * wd) / (MIN_CU_SIZE * MIN_CU_SIZE);
        qp_size = num_8x8;

  
        size += vert_bs_size;

  
        size += horz_bs_size;

  
        size += qp_size;

  
        size += qp_const_flag_size;

        pu1_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
        RETURN_IF((NULL == pu1_buf), IV_FAIL);

        memset(pu1_buf, 0, size);

 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].s_bs_ctxt.pu4_pic_vert_bs = (UWORD32 *)pu1_buf;
            ps_codec->as_process[i].s_deblk_ctxt.s_bs_ctxt.pu4_pic_vert_bs = (UWORD32 *)pu1_buf;
            ps_codec->s_parse.s_deblk_ctxt.s_bs_ctxt.pu4_pic_vert_bs = (UWORD32 *)pu1_buf;
            pu1_buf += vert_bs_size;

            ps_codec->as_process[i].s_bs_ctxt.pu4_pic_horz_bs = (UWORD32 *)pu1_buf;
            ps_codec->as_process[i].s_deblk_ctxt.s_bs_ctxt.pu4_pic_horz_bs = (UWORD32 *)pu1_buf;
            ps_codec->s_parse.s_deblk_ctxt.s_bs_ctxt.pu4_pic_horz_bs = (UWORD32 *)pu1_buf;
            pu1_buf += horz_bs_size;

            ps_codec->as_process[i].s_bs_ctxt.pu1_pic_qp = (UWORD8 *)pu1_buf;
            ps_codec->as_process[i].s_deblk_ctxt.s_bs_ctxt.pu1_pic_qp = (UWORD8 *)pu1_buf;
            ps_codec->s_parse.s_deblk_ctxt.s_bs_ctxt.pu1_pic_qp = (UWORD8 *)pu1_buf;
            pu1_buf += qp_size;

            ps_codec->as_process[i].s_bs_ctxt.pu1_pic_qp_const_in_ctb = (UWORD8 *)pu1_buf;
            ps_codec->as_process[i].s_deblk_ctxt.s_bs_ctxt.pu1_pic_qp_const_in_ctb = (UWORD8 *)pu1_buf;
            ps_codec->s_parse.s_deblk_ctxt.s_bs_ctxt.pu1_pic_qp_const_in_ctb = (UWORD8 *)pu1_buf;
            pu1_buf += qp_const_flag_size;

            pu1_buf -= (vert_bs_size + horz_bs_size + qp_size + qp_const_flag_size);
 }
        ps_codec->s_parse.s_bs_ctxt.pu4_pic_vert_bs = (UWORD32 *)pu1_buf;
        pu1_buf += vert_bs_size;

        ps_codec->s_parse.s_bs_ctxt.pu4_pic_horz_bs = (UWORD32 *)pu1_buf;
        pu1_buf += horz_bs_size;

        ps_codec->s_parse.s_bs_ctxt.pu1_pic_qp = (UWORD8 *)pu1_buf;
        pu1_buf += qp_size;

        ps_codec->s_parse.s_bs_ctxt.pu1_pic_qp_const_in_ctb = (UWORD8 *)pu1_buf;
        pu1_buf += qp_const_flag_size;

 }

  
    size  = wd / MIN_CTB_SIZE;
  
    size *= (ht / MIN_CTB_SIZE + 2)  ;

    size *= sizeof(UWORD16);
    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);

    ps_codec->pu1_tile_idx_base = pv_buf;
 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
        ps_codec->as_process[i].pu1_tile_idx = (UWORD16 *)pv_buf + wd / MIN_CTB_SIZE  ;
 }

  
    size = 3 * 4;

  
    size *= wd / MIN_CTB_SIZE;

  
    size *= ht / MIN_CTB_SIZE;
    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);

    ps_codec->s_parse.ps_pic_sao = (sao_t *)pv_buf;
    ps_codec->s_parse.s_sao_ctxt.ps_pic_sao = (sao_t *)pv_buf;
 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
        ps_codec->as_process[i].s_sao_ctxt.ps_pic_sao = ps_codec->s_parse.ps_pic_sao;
 }

  
    ps_codec->pu1_bitsbuf_dynamic = NULL;
    size = wd * ht;
 if(size > MIN_BITSBUF_SIZE)
 {
        pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size + 16);  
        RETURN_IF((NULL == pv_buf), IV_FAIL);
        ps_codec->pu1_bitsbuf_dynamic = pv_buf;
        ps_codec->u4_bitsbuf_size_dynamic = size;
 }

    size = ihevcd_get_tu_data_size(wd * ht);
    pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
    RETURN_IF((NULL == pv_buf), IV_FAIL);
    memset(pv_buf, 0, size);
    ps_codec->pv_tu_data = pv_buf;

 {
 sps_t *ps_sps = (ps_codec->s_parse.ps_sps_base + ps_codec->i4_sps_id);


  
  
        max_dpb_size = ps_sps->ai1_sps_max_dec_pic_buffering[ps_sps->i1_sps_max_sub_layers - 1];
  
        size = (max_dpb_size + 1) * sizeof(mv_buf_t);

        size += (max_dpb_size + 1) *
                        ihevcd_get_pic_mv_bank_size(wd * ht);

        pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
        RETURN_IF((NULL == pv_buf), IV_FAIL);

        ps_codec->pv_mv_bank_buf_base = pv_buf;
        ps_codec->i4_total_mv_bank_size = size;

 }

  
  
 if(0 == ps_codec->i4_share_disp_buf)
 {
  
        size = ihevcd_get_total_pic_buf_size(ps_codec, wd, ht);
        pv_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
        RETURN_IF((NULL == pv_buf), IV_FAIL);


        ps_codec->i4_total_pic_buf_size = size;
        ps_codec->pu1_ref_pic_buf_base = (UWORD8 *)pv_buf;
 }

    ps_codec->pv_proc_jobq = ihevcd_jobq_init(ps_codec->pv_proc_jobq_buf, ps_codec->i4_proc_jobq_buf_size);
    RETURN_IF((ps_codec->pv_proc_jobq == NULL), IV_FAIL);

  
    ps_codec->s_parse.pv_proc_jobq = ps_codec->pv_proc_jobq;
 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
        ps_codec->as_process[i].pv_proc_jobq = ps_codec->pv_proc_jobq;
        ps_codec->as_process[i].i4_id = i;
        ps_codec->as_process[i].ps_codec = ps_codec;

  
        ps_codec->as_process[i].i4_check_parse_status = 0;
        ps_codec->as_process[i].i4_check_proc_status = 0;
 }

    ps_codec->u4_allocate_dynamic_done = 1;

 return IV_SUCCESS;
}
