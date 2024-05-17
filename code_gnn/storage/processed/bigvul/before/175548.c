IHEVCD_ERROR_T ihevcd_pic_buf_mgr_add_bufs(codec_t *ps_codec)
{
    IHEVCD_ERROR_T ret = (IHEVCD_ERROR_T)IHEVCD_SUCCESS;
    WORD32 i;
    WORD32 max_dpb_size;
 sps_t *ps_sps;
    UWORD8 *pu1_buf;
 pic_buf_t *ps_pic_buf;
    WORD32 pic_buf_size_allocated;




  
    ps_sps = ps_codec->s_parse.ps_sps;

  
    max_dpb_size = ps_sps->ai1_sps_max_dec_pic_buffering[ps_sps->i1_sps_max_sub_layers - 1];

 if(ps_codec->e_frm_out_mode != IVD_DECODE_FRAME_OUT)
        max_dpb_size += ps_sps->ai1_sps_max_num_reorder_pics[ps_sps->i1_sps_max_sub_layers - 1];

  
    max_dpb_size++;


    pu1_buf = (UWORD8 *)ps_codec->pu1_ref_pic_buf_base;

    ps_pic_buf = (pic_buf_t *)ps_codec->ps_pic_buf;

  
 if(0 == ps_codec->i4_share_disp_buf)
 {
        WORD32 buf_ret;
        WORD32 luma_samples;
        WORD32 chroma_samples;
        pic_buf_size_allocated = ps_codec->i4_total_pic_buf_size;

        luma_samples = (ps_codec->i4_strd) *
 (ps_sps->i2_pic_height_in_luma_samples + PAD_HT);

        chroma_samples = luma_samples / 2;

  
  
 for(i = 0; i < max_dpb_size; i++)
 {
            pic_buf_size_allocated -= (luma_samples + chroma_samples);

 if(pic_buf_size_allocated < 0)
 {
                ps_codec->s_parse.i4_error_code = IHEVCD_INSUFFICIENT_MEM_PICBUF;
 return IHEVCD_INSUFFICIENT_MEM_PICBUF;
 }

            ps_pic_buf->pu1_luma = pu1_buf + ps_codec->i4_strd * PAD_TOP + PAD_LEFT;
            pu1_buf += luma_samples;

            ps_pic_buf->pu1_chroma = pu1_buf + ps_codec->i4_strd * (PAD_TOP / 2) + PAD_LEFT;
            pu1_buf += chroma_samples;

            buf_ret = ihevc_buf_mgr_add((buf_mgr_t *)ps_codec->pv_pic_buf_mgr, ps_pic_buf, i);


 if(0 != buf_ret)
 {
                ps_codec->s_parse.i4_error_code = IHEVCD_BUF_MGR_ERROR;
 return IHEVCD_BUF_MGR_ERROR;
 }
            ps_pic_buf++;
 }
 }
 else
 {
  

 for(i = 0; i < BUF_MGR_MAX_CNT; i++)
 {
            ps_pic_buf = ihevc_buf_mgr_get_buf((buf_mgr_t *)ps_codec->pv_pic_buf_mgr, i);
 if((NULL == ps_pic_buf) ||
 (NULL == ps_pic_buf->pu1_luma) ||
 (NULL == ps_pic_buf->pu1_chroma))
 {
 break;
 }
            ps_pic_buf->pu1_luma += ps_codec->i4_strd * PAD_TOP + PAD_LEFT;
            ps_pic_buf->pu1_chroma += ps_codec->i4_strd * (PAD_TOP / 2) + PAD_LEFT;
 }
 }

 return ret;
}
