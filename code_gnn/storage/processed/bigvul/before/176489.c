WORD32 ihevcd_set_display_frame(iv_obj_t *ps_codec_obj,
 void *pv_api_ip,
 void *pv_api_op)
{
    WORD32 ret = IV_SUCCESS;

 ivd_set_display_frame_ip_t *ps_dec_disp_ip;
 ivd_set_display_frame_op_t *ps_dec_disp_op;

    WORD32 i;

 codec_t *ps_codec = (codec_t *)(ps_codec_obj->pv_codec_handle);

    ps_dec_disp_ip = (ivd_set_display_frame_ip_t *)pv_api_ip;
    ps_dec_disp_op = (ivd_set_display_frame_op_t *)pv_api_op;

    ps_codec->i4_num_disp_bufs = 0;
 if(ps_codec->i4_share_disp_buf)
 {
        UWORD32 num_bufs = ps_dec_disp_ip->num_disp_bufs;
 pic_buf_t *ps_pic_buf;
        UWORD8 *pu1_buf;
        WORD32 buf_ret;

        UWORD8 *pu1_chroma_buf = NULL;
        num_bufs = MIN(num_bufs, BUF_MGR_MAX_CNT);
        ps_codec->i4_num_disp_bufs = num_bufs;

        ps_pic_buf = (pic_buf_t *)ps_codec->ps_pic_buf;

  
 if(ps_codec->e_chroma_fmt == IV_YUV_420P)
 {
            WORD32 num_samples = ps_dec_disp_ip->s_disp_buffer[0].u4_min_out_buf_size[1] << 1;
            WORD32 size = num_samples * num_bufs;
 void *pv_mem_ctxt = ps_codec->pv_mem_ctxt;

            pu1_chroma_buf = ps_codec->pf_aligned_alloc(pv_mem_ctxt, 128, size);
            RETURN_IF((NULL == pu1_chroma_buf), IV_FAIL);

            ps_codec->pu1_cur_chroma_ref_buf = pu1_chroma_buf;
 }
 for(i = 0; i < (WORD32)num_bufs; i++)
 {
  
            pu1_buf =  ps_dec_disp_ip->s_disp_buffer[i].pu1_bufs[0];
            ps_pic_buf->pu1_luma = pu1_buf;

 if(ps_codec->e_chroma_fmt == IV_YUV_420P)
 {
                pu1_buf = pu1_chroma_buf;
                pu1_chroma_buf += ps_dec_disp_ip->s_disp_buffer[0].u4_min_out_buf_size[1] << 1;
 }
 else
 {
  
                pu1_buf =  ps_dec_disp_ip->s_disp_buffer[i].pu1_bufs[1];
 }

            ps_pic_buf->pu1_chroma = pu1_buf;

            buf_ret = ihevc_buf_mgr_add((buf_mgr_t *)ps_codec->pv_pic_buf_mgr, ps_pic_buf, i);

 if(0 != buf_ret)
 {
                ps_codec->i4_error_code = IHEVCD_BUF_MGR_ERROR;
 return IHEVCD_BUF_MGR_ERROR;
 }

  
  
            ihevc_buf_mgr_set_status((buf_mgr_t *)ps_codec->pv_pic_buf_mgr, i, BUF_MGR_DISP);

            ps_pic_buf++;

  
            memcpy(&ps_codec->s_disp_buffer[ps_codec->i4_share_disp_buf_cnt],
 &ps_dec_disp_ip->s_disp_buffer[i],
 sizeof(ps_dec_disp_ip->s_disp_buffer[i]));

            ps_codec->i4_share_disp_buf_cnt++;

 }
 }

    ps_dec_disp_op->u4_error_code = 0;
 return ret;

}
