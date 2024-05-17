WORD32 ihevcd_init(codec_t *ps_codec)
{
    WORD32 status = IV_SUCCESS;
    WORD32 i;

  
    ihevcd_free_dynamic_bufs(ps_codec);

    ps_codec->u4_allocate_dynamic_done = 0;
    ps_codec->i4_num_disp_bufs = 1;
    ps_codec->i4_flush_mode = 0;

    ps_codec->i4_ht = ps_codec->i4_disp_ht = 0;
    ps_codec->i4_wd = ps_codec->i4_disp_wd = 0;
    ps_codec->i4_strd = 0;
    ps_codec->i4_disp_strd = 0;
    ps_codec->i4_num_cores = 1;

    ps_codec->u4_pic_cnt = 0;
    ps_codec->u4_disp_cnt = 0;

    ps_codec->i4_header_mode = 0;
    ps_codec->i4_header_in_slice_mode = 0;
    ps_codec->i4_sps_done = 0;
    ps_codec->i4_pps_done = 0;
    ps_codec->i4_init_done   = 1;
    ps_codec->i4_first_pic_done = 0;
    ps_codec->s_parse.i4_first_pic_init = 0;
    ps_codec->i4_error_code = 0;
    ps_codec->i4_reset_flag = 0;
    ps_codec->i4_cra_as_first_pic = 1;
    ps_codec->i4_rasl_output_flag = 0;

    ps_codec->i4_prev_poc_msb = 0;
    ps_codec->i4_prev_poc_lsb = -1;
    ps_codec->i4_max_prev_poc_lsb = -1;
    ps_codec->s_parse.i4_abs_pic_order_cnt = -1;

  
    ps_codec->e_ref_chroma_fmt = IV_YUV_420SP_UV;

  
 if(IV_YUV_420SP_VU == ps_codec->e_chroma_fmt)
 {
        ps_codec->e_ref_chroma_fmt = IV_YUV_420SP_VU;
 }



    ps_codec->i4_disable_deblk_pic = 0;

    ps_codec->i4_degrade_pic_cnt    = 0;
    ps_codec->i4_degrade_pics       = 0;
    ps_codec->i4_degrade_type       = 0;
    ps_codec->i4_disable_sao_pic    = 0;
    ps_codec->i4_fullpel_inter_pred = 0;
    ps_codec->u4_enable_fmt_conv_ahead = 0;
    ps_codec->i4_share_disp_buf_cnt = 0;

 {
 sps_t *ps_sps = ps_codec->ps_sps_base;
 pps_t *ps_pps = ps_codec->ps_pps_base;

 for(i = 0; i < MAX_SPS_CNT; i++)
 {
            ps_sps->i1_sps_valid = 0;
            ps_sps++;
 }

 for(i = 0; i < MAX_PPS_CNT; i++)
 {
            ps_pps->i1_pps_valid = 0;
            ps_pps++;
 }
 }

    ihevcd_set_default_params(ps_codec);
  
    ihevc_buf_mgr_init((buf_mgr_t *)ps_codec->pv_mv_buf_mgr);

  
    ihevc_buf_mgr_init((buf_mgr_t *)ps_codec->pv_pic_buf_mgr);

    ps_codec->ps_pic_buf = (pic_buf_t *)ps_codec->pv_pic_buf_base;

    memset(ps_codec->ps_pic_buf, 0, BUF_MGR_MAX_CNT  * sizeof(pic_buf_t));



  
    ihevc_disp_mgr_init((disp_mgr_t *)ps_codec->pv_disp_buf_mgr);

  
    ihevc_dpb_mgr_init((dpb_mgr_t *)ps_codec->pv_dpb_mgr);

    ps_codec->e_processor_soc = SOC_GENERIC;
  
    ps_codec->u4_nctb = 0x7FFFFFFF;
    ihevcd_init_arch(ps_codec);

    ihevcd_init_function_ptr(ps_codec);

    ihevcd_update_function_ptr(ps_codec);

 return status;
}
