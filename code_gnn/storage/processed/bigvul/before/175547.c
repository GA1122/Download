IHEVCD_ERROR_T ihevcd_parse_pic_init(codec_t *ps_codec)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
 mv_buf_t *ps_mv_buf;
 sps_t *ps_sps;
    WORD32 num_min_cu;
    WORD32 cur_pic_buf_id;
    WORD32 cur_mv_bank_buf_id;
 pic_buf_t *ps_cur_pic;
 slice_header_t *ps_slice_hdr;
    UWORD8 *pu1_cur_pic_luma, *pu1_cur_pic_chroma;
    WORD32 i;

    ps_codec->s_parse.i4_error_code = IHEVCD_SUCCESS;
    ps_sps = ps_codec->s_parse.ps_sps;
    ps_slice_hdr = ps_codec->s_parse.ps_slice_hdr;
  
    ps_codec->i4_pic_present = 1;

  
    num_min_cu = ((ps_sps->i2_pic_height_in_luma_samples + 7) / 8) * ((ps_sps->i2_pic_width_in_luma_samples + 63) / 64);
    memset(ps_codec->s_parse.pu1_pic_intra_flag, 0, num_min_cu);
    memset(ps_codec->s_parse.pu1_pic_no_loop_filter_flag, 0, num_min_cu);



 if(0 == ps_codec->s_parse.i4_first_pic_init)
 {
        ret = ihevcd_mv_buf_mgr_add_bufs(ps_codec);
        RETURN_IF((ret != (IHEVCD_ERROR_T)IHEVCD_SUCCESS), ret);

        ret = ihevcd_pic_buf_mgr_add_bufs(ps_codec);
        RETURN_IF((ret != (IHEVCD_ERROR_T)IHEVCD_SUCCESS), ret);

        ps_codec->s_parse.i4_first_pic_init = 1;
 }

  
 {
        WORD32 slice_idx;
        WORD32 slice_start_idx;

        slice_start_idx = ps_codec->i4_slice_error ? 2 : 1;

 for(slice_idx = slice_start_idx; slice_idx < MAX_SLICE_HDR_CNT; slice_idx++)
 {
 slice_header_t *ps_slice_hdr_tmp = ps_codec->ps_slice_hdr_base + slice_idx;
            ps_slice_hdr_tmp->i2_ctb_x = -1;
            ps_slice_hdr_tmp->i2_ctb_y = -1;

 }
 }

  
 {
        ps_mv_buf = (mv_buf_t *)ihevc_buf_mgr_get_next_free((buf_mgr_t *)ps_codec->pv_mv_buf_mgr, &cur_mv_bank_buf_id);

  
 if(NULL == ps_mv_buf)
 {
            ps_codec->s_parse.i4_error_code = IHEVCD_NO_FREE_MVBANK;
            ps_codec->i4_error_code = IHEVCD_NO_FREE_MVBANK;
 return IHEVCD_NO_FREE_MVBANK;
 }

        ps_codec->s_parse.ps_cur_mv_buf = ps_mv_buf;
  
        ps_mv_buf->i4_abs_poc = ps_slice_hdr->i4_abs_pic_order_cnt;
 }

  
  
 {

        UWORD8 *pu1_buf;
        ps_cur_pic = (pic_buf_t *)ihevc_buf_mgr_get_next_free((buf_mgr_t *)ps_codec->pv_pic_buf_mgr, &cur_pic_buf_id);

  
 if(NULL == ps_cur_pic)
 {
            ps_codec->s_parse.i4_error_code = IHEVCD_NO_FREE_PICBUF;
            ps_codec->i4_error_code = IHEVCD_NO_FREE_PICBUF;
 return IHEVCD_NO_FREE_PICBUF;
 }

  
        ps_cur_pic->u4_ts = ps_codec->u4_ts;
        ps_cur_pic->i4_abs_poc = ps_slice_hdr->i4_abs_pic_order_cnt;
        ps_cur_pic->i4_poc_lsb = ps_slice_hdr->i4_pic_order_cnt_lsb;
        pu1_buf = ps_cur_pic->pu1_luma;
        pu1_cur_pic_luma = pu1_buf;

        pu1_buf = ps_cur_pic->pu1_chroma;

        pu1_cur_pic_chroma = pu1_buf;
 }

 if(0 == ps_codec->u4_pic_cnt)
 {
        memset(ps_cur_pic->pu1_luma, 128, (ps_sps->i2_pic_width_in_luma_samples + PAD_WD) * ps_sps->i2_pic_height_in_luma_samples);
        memset(ps_cur_pic->pu1_chroma, 128, (ps_sps->i2_pic_width_in_luma_samples + PAD_WD) * ps_sps->i2_pic_height_in_luma_samples / 2);
 }

  
 {
 pic_buf_t *ps_pic_buf_ref;
 mv_buf_t *ps_mv_buf_ref;
        WORD32 r_idx;
 dpb_mgr_t *ps_dpb_mgr = (dpb_mgr_t *)ps_codec->pv_dpb_mgr;
 buf_mgr_t *ps_mv_buf_mgr = (buf_mgr_t *)ps_codec->pv_mv_buf_mgr;

        ps_pic_buf_ref = ihevc_dpb_mgr_get_ref_by_nearest_poc(ps_dpb_mgr, ps_slice_hdr->i4_abs_pic_order_cnt);
 if(NULL == ps_pic_buf_ref)
 {
            WORD32 size;

            WORD32 num_pu;
            WORD32 num_ctb;
            WORD32 pic_size;
  
            pic_size = ALIGN64(ps_sps->i2_pic_width_in_luma_samples) *
                            ALIGN64(ps_sps->i2_pic_height_in_luma_samples);

            num_pu = pic_size / (MIN_PU_SIZE * MIN_PU_SIZE);
            num_ctb = pic_size / (MIN_CTB_SIZE * MIN_CTB_SIZE);

            memset(ps_mv_buf->ai4_l0_collocated_poc, 0, sizeof(ps_mv_buf->ai4_l0_collocated_poc));
            memset(ps_mv_buf->ai1_l0_collocated_poc_lt, 0, sizeof(ps_mv_buf->ai1_l0_collocated_poc_lt));
            memset(ps_mv_buf->ai4_l1_collocated_poc, 0, sizeof(ps_mv_buf->ai4_l1_collocated_poc));
            memset(ps_mv_buf->ai1_l1_collocated_poc_lt, 0, sizeof(ps_mv_buf->ai1_l1_collocated_poc_lt));

            size = (num_ctb + 1) * sizeof(WORD32);
            memset(ps_mv_buf->pu4_pic_pu_idx, 0, size);

            size = num_pu;
            memset(ps_mv_buf->pu1_pic_pu_map, 0, size);
            size = ALIGN4(num_ctb * sizeof(UWORD16));
            memset(ps_mv_buf->pu1_pic_slice_map, 0, size);
            size = num_pu * sizeof(pu_t);
            memset(ps_mv_buf->ps_pic_pu, 0, size);

            ps_pic_buf_ref = ps_cur_pic;
            ps_mv_buf_ref = ps_mv_buf;
 }
 else
 {
            ps_mv_buf_ref = ihevcd_mv_mgr_get_poc(ps_mv_buf_mgr, ps_pic_buf_ref->i4_abs_poc);
 }

 for(r_idx = 0; r_idx < ps_slice_hdr->i1_num_ref_idx_l0_active; r_idx++)
 {
 if(NULL == ps_slice_hdr->as_ref_pic_list0[r_idx].pv_pic_buf)
 {
                ps_slice_hdr->as_ref_pic_list0[r_idx].pv_pic_buf = (void *)ps_pic_buf_ref;
                ps_slice_hdr->as_ref_pic_list0[r_idx].pv_mv_buf = (void *)ps_mv_buf_ref;
 }
 }

 for(r_idx = ps_slice_hdr->i1_num_ref_idx_l0_active; r_idx < MAX_DPB_SIZE; r_idx++)
 {
            ps_slice_hdr->as_ref_pic_list0[r_idx].pv_pic_buf = (void *)ps_pic_buf_ref;
            ps_slice_hdr->as_ref_pic_list0[r_idx].pv_mv_buf = (void *)ps_mv_buf_ref;
 }

 for(r_idx = 0; r_idx < ps_slice_hdr->i1_num_ref_idx_l1_active; r_idx++)
 {
 if(NULL == ps_slice_hdr->as_ref_pic_list1[r_idx].pv_pic_buf)
 {
                ps_slice_hdr->as_ref_pic_list1[r_idx].pv_pic_buf = (void *)ps_pic_buf_ref;
                ps_slice_hdr->as_ref_pic_list1[r_idx].pv_mv_buf = (void *)ps_mv_buf_ref;
 }
 }

 for(r_idx = ps_slice_hdr->i1_num_ref_idx_l1_active; r_idx < MAX_DPB_SIZE; r_idx++)
 {
            ps_slice_hdr->as_ref_pic_list1[r_idx].pv_pic_buf = (void *)ps_pic_buf_ref;
            ps_slice_hdr->as_ref_pic_list1[r_idx].pv_mv_buf = (void *)ps_mv_buf_ref;
 }
 }


  
    ihevcd_jobq_reset((jobq_t *)ps_codec->pv_proc_jobq);

    ps_codec->s_parse.i4_pic_pu_idx = 0;
    ps_codec->s_parse.i4_pic_tu_idx = 0;

    ps_codec->s_parse.pu1_pic_pu_map = ps_mv_buf->pu1_pic_pu_map;
    ps_codec->s_parse.ps_pic_pu      = ps_mv_buf->ps_pic_pu;
    ps_codec->s_parse.pu4_pic_pu_idx = ps_mv_buf->pu4_pic_pu_idx;
    ps_codec->s_parse.pu1_slice_idx = (UWORD16 *)ps_mv_buf->pu1_pic_slice_map;
 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
        ps_codec->as_process[i].pu1_slice_idx = (UWORD16 *)ps_mv_buf->pu1_pic_slice_map;
 }
    ps_codec->s_parse.pu1_pu_map = ps_codec->s_parse.pu1_pic_pu_map;
    ps_codec->s_parse.ps_pu = ps_codec->s_parse.ps_pic_pu;

 {
        UWORD8 *pu1_buf;
        WORD32 ctb_luma_min_tu_cnt, ctb_chroma_min_tu_cnt, ctb_min_tu_cnt;
        WORD32 pic_size;
        WORD32 num_ctb;

        pic_size = ALIGN64(ps_sps->i2_pic_width_in_luma_samples) *
                        ALIGN64(ps_sps->i2_pic_height_in_luma_samples);

        ctb_luma_min_tu_cnt = pic_size / (MIN_TU_SIZE * MIN_TU_SIZE);

        ctb_chroma_min_tu_cnt = ctb_luma_min_tu_cnt >> 1;

        ctb_min_tu_cnt = ctb_luma_min_tu_cnt + ctb_chroma_min_tu_cnt;

        num_ctb = pic_size / (MIN_CTB_SIZE * MIN_CTB_SIZE);
        pu1_buf  = (UWORD8 *)ps_codec->pv_tu_data;
        ps_codec->s_parse.pu4_pic_tu_idx = (UWORD32 *)pu1_buf;
        pu1_buf += (num_ctb + 1) * sizeof(WORD32);

        ps_codec->s_parse.pu1_pic_tu_map = pu1_buf;
        pu1_buf += ctb_min_tu_cnt;

        ps_codec->s_parse.ps_pic_tu = (tu_t *)pu1_buf;
        pu1_buf += ctb_min_tu_cnt * sizeof(tu_t);

        ps_codec->s_parse.pv_pic_tu_coeff_data = pu1_buf;

        ps_codec->s_parse.pu1_tu_map = ps_codec->s_parse.pu1_pic_tu_map;
        ps_codec->s_parse.ps_tu = ps_codec->s_parse.ps_pic_tu;
        ps_codec->s_parse.pv_tu_coeff_data = ps_codec->s_parse.pv_pic_tu_coeff_data;
 }

    ps_codec->s_parse.s_bs_ctxt.ps_pic_pu = ps_codec->s_parse.ps_pic_pu;
    ps_codec->s_parse.s_bs_ctxt.pu4_pic_pu_idx = ps_codec->s_parse.pu4_pic_pu_idx;
    ps_codec->s_parse.s_bs_ctxt.pu4_pic_tu_idx = ps_codec->s_parse.pu4_pic_tu_idx;


  
    ps_codec->i4_proc_nctb = ihevcd_nctb_cnt(ps_codec, ps_sps);

  
 {
        WORD32 num_ctb;

        num_ctb = ps_sps->i4_pic_size_in_ctb;

        memset(ps_codec->pu1_parse_map, 0, num_ctb);

        memset(ps_codec->pu1_proc_map, 0, num_ctb);
 }



  
    ps_codec->i4_disp_buf_id = -1;
    ps_codec->ps_disp_buf = NULL;

    ps_codec->i4_disable_deblk_pic  = 0;
    ps_codec->i4_disable_sao_pic    = 0;
    ps_codec->i4_fullpel_inter_pred = 0;
    ps_codec->i4_mv_frac_mask       = 0x7FFFFFFF;

  
 if(ps_codec->i4_degrade_type && ps_codec->i4_degrade_pics)
 {
        WORD32 degrade_pic;
        ps_codec->i4_degrade_pic_cnt++;
        degrade_pic = 0;

  
 switch(ps_codec->i4_degrade_pics)
 {
 case 4:
 {
                degrade_pic = 1;
 break;
 }
 case 3:
 {
 if(ps_slice_hdr->i1_slice_type != ISLICE)
                    degrade_pic = 1;

 break;
 }
 case 2:
 {

  
 if((ps_slice_hdr->i1_slice_type != ISLICE) &&
 (ps_codec->i4_degrade_pic_cnt != ps_codec->i4_nondegrade_interval))
                    degrade_pic = 1;

 break;
 }
 case 1:
 {
  
 if((ps_slice_hdr->i1_nal_unit_type < NAL_BLA_W_LP) &&
 (ps_slice_hdr->i1_nal_unit_type % 2 == 0))
 {
                    degrade_pic = 1;
 }
 break;
 }


 }
 if(degrade_pic)
 {
 if(ps_codec->i4_degrade_type & 0x1)
                ps_codec->i4_disable_sao_pic = 1;

 if(ps_codec->i4_degrade_type & 0x2)
                ps_codec->i4_disable_deblk_pic = 1;

  
 if((ps_slice_hdr->i1_nal_unit_type < NAL_BLA_W_LP) &&
 (ps_slice_hdr->i1_nal_unit_type % 2 == 0))
 {
 if(ps_codec->i4_degrade_type & 0x4)
                    ps_codec->i4_mv_frac_mask = 0;

 if(ps_codec->i4_degrade_type & 0x8)
                    ps_codec->i4_mv_frac_mask = 0;
 }
 }
 else
            ps_codec->i4_degrade_pic_cnt = 0;
 }


 {
        WORD32 i;
 for(i = 0; i < MAX_PROCESS_THREADS; i++)
 {
            ps_codec->as_process[i].pu4_pic_pu_idx = ps_codec->s_parse.pu4_pic_pu_idx;
            ps_codec->as_process[i].ps_pic_pu = ps_codec->s_parse.ps_pic_pu;
            ps_codec->as_process[i].pu1_pic_pu_map = ps_codec->s_parse.pu1_pic_pu_map;
            ps_codec->as_process[i].pu4_pic_tu_idx = ps_codec->s_parse.pu4_pic_tu_idx;
            ps_codec->as_process[i].ps_pic_tu = ps_codec->s_parse.ps_pic_tu;
            ps_codec->as_process[i].pu1_pic_tu_map = ps_codec->s_parse.pu1_pic_tu_map;
            ps_codec->as_process[i].pv_pic_tu_coeff_data = ps_codec->s_parse.pv_pic_tu_coeff_data;
            ps_codec->as_process[i].i4_cur_mv_bank_buf_id = cur_mv_bank_buf_id;
            ps_codec->as_process[i].s_sao_ctxt.pu1_slice_idx = ps_codec->as_process[i].pu1_slice_idx;
            ps_codec->as_process[i].s_sao_ctxt.pu1_tile_idx = ps_codec->as_process[i].pu1_tile_idx;

  
            ps_codec->as_process[i].pu1_cur_pic_luma = pu1_cur_pic_luma;
            ps_codec->as_process[i].pu1_cur_pic_chroma = pu1_cur_pic_chroma;
            ps_codec->as_process[i].ps_cur_pic = ps_cur_pic;
            ps_codec->as_process[i].i4_cur_pic_buf_id = cur_pic_buf_id;

            ps_codec->as_process[i].ps_out_buffer = ps_codec->ps_out_buffer;
 if(1 < ps_codec->i4_num_cores)
 {
                ps_codec->as_process[i].i4_check_parse_status = 1;
                ps_codec->as_process[i].i4_check_proc_status = 1;
 }
 else
 {
                ps_codec->as_process[i].i4_check_parse_status = 0;
                ps_codec->as_process[i].i4_check_proc_status = 0;
 }
            ps_codec->as_process[i].pu1_pic_intra_flag = ps_codec->s_parse.pu1_pic_intra_flag;
            ps_codec->as_process[i].pu1_pic_no_loop_filter_flag = ps_codec->s_parse.pu1_pic_no_loop_filter_flag;
            ps_codec->as_process[i].i4_init_done = 0;

            ps_codec->as_process[i].s_bs_ctxt.pu4_pic_tu_idx = ps_codec->as_process[i].pu4_pic_tu_idx;
            ps_codec->as_process[i].s_bs_ctxt.pu4_pic_pu_idx = ps_codec->as_process[i].pu4_pic_pu_idx;
            ps_codec->as_process[i].s_bs_ctxt.ps_pic_pu = ps_codec->as_process[i].ps_pic_pu;
            ps_codec->as_process[i].s_deblk_ctxt.pu1_pic_no_loop_filter_flag = ps_codec->s_parse.pu1_pic_no_loop_filter_flag;
            ps_codec->as_process[i].s_deblk_ctxt.pu1_cur_pic_luma = pu1_cur_pic_luma;
            ps_codec->as_process[i].s_deblk_ctxt.pu1_cur_pic_chroma = pu1_cur_pic_chroma;
            ps_codec->as_process[i].s_sao_ctxt.pu1_pic_no_loop_filter_flag = ps_codec->s_parse.pu1_pic_no_loop_filter_flag;
            ps_codec->as_process[i].s_sao_ctxt.pu1_cur_pic_luma = pu1_cur_pic_luma;
            ps_codec->as_process[i].s_sao_ctxt.pu1_cur_pic_chroma = pu1_cur_pic_chroma;
 if(i < (ps_codec->i4_num_cores - 1))
 {
                ithread_create(ps_codec->apv_process_thread_handle[i], NULL,
 (void *)ihevcd_process_thread,
 (void *)&ps_codec->as_process[i]);
                ps_codec->ai4_process_thread_created[i] = 1;
 }
 else
 {
                ps_codec->ai4_process_thread_created[i] = 0;
 }

 }
        ps_codec->s_parse.s_deblk_ctxt.pu1_cur_pic_luma = pu1_cur_pic_luma;
        ps_codec->s_parse.s_deblk_ctxt.pu1_cur_pic_chroma = pu1_cur_pic_chroma;

        ps_codec->s_parse.s_sao_ctxt.pu1_cur_pic_luma = pu1_cur_pic_luma;
        ps_codec->s_parse.s_sao_ctxt.pu1_cur_pic_chroma = pu1_cur_pic_chroma;
 }
  
  
 {
        WORD32 abs_poc;
 slice_header_t *ps_slice_hdr;
        ps_slice_hdr = ps_codec->s_parse.ps_slice_hdr;
        abs_poc = ps_slice_hdr->i4_abs_pic_order_cnt;
        ihevc_disp_mgr_add((disp_mgr_t *)ps_codec->pv_disp_buf_mgr,
                           ps_codec->as_process[0].i4_cur_pic_buf_id,
                           abs_poc,
                           ps_codec->as_process[0].ps_cur_pic);
 }
    ps_codec->ps_disp_buf = NULL;
  
  
 if(((WORD32)(ps_codec->u4_pic_cnt - ps_codec->u4_disp_cnt) >= ps_sps->ai1_sps_max_num_reorder_pics[ps_sps->i1_sps_max_sub_layers - 1]) ||
 (ps_codec->e_frm_out_mode == IVD_DECODE_FRAME_OUT))

 {
        ps_codec->ps_disp_buf = (pic_buf_t *)ihevc_disp_mgr_get((disp_mgr_t *)ps_codec->pv_disp_buf_mgr, &ps_codec->i4_disp_buf_id);
        ps_codec->u4_disp_cnt++;
 }

    ps_codec->s_fmt_conv.i4_cur_row = 0;
  
    ps_codec->s_fmt_conv.i4_num_rows = 4;

 if(ps_codec->u4_enable_fmt_conv_ahead && (ps_codec->i4_num_cores > 1))
 {
 process_ctxt_t *ps_proc;

  
        ps_proc = &ps_codec->as_process[ps_codec->i4_num_cores - 1];

  
 if((ps_codec->ps_disp_buf) && (ps_codec->i4_disp_buf_id != ps_proc->i4_cur_pic_buf_id) &&
 ((0 == ps_codec->i4_share_disp_buf) || (IV_YUV_420P == ps_codec->e_chroma_fmt)))
 {

 for(i = 0; i < ps_sps->i2_pic_ht_in_ctb; i++)
 {
 proc_job_t s_job;
                IHEVCD_ERROR_T ret;
                s_job.i4_cmd = CMD_FMTCONV;
                s_job.i2_ctb_cnt = 0;
                s_job.i2_ctb_x = 0;
                s_job.i2_ctb_y = i;
                s_job.i2_slice_idx = 0;
                s_job.i4_tu_coeff_data_ofst = 0;
                ret = ihevcd_jobq_queue((jobq_t *)ps_codec->s_parse.pv_proc_jobq,
 &s_job, sizeof(proc_job_t), 1);
 if(ret != (IHEVCD_ERROR_T)IHEVCD_SUCCESS)
 return ret;
 }
 }
 }


 return ret;
}
