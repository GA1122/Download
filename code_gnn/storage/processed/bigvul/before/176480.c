WORD32 ihevcd_get_buf_info(iv_obj_t *ps_codec_obj,
 void *pv_api_ip,
 void *pv_api_op)
{

 codec_t *ps_codec;
    UWORD32 i = 0;
    WORD32 wd, ht;
 ivd_ctl_getbufinfo_op_t *ps_ctl_op =
 (ivd_ctl_getbufinfo_op_t *)pv_api_op;

    UNUSED(pv_api_ip);
    ps_ctl_op->u4_error_code = 0;

    ps_codec = (codec_t *)(ps_codec_obj->pv_codec_handle);

    ps_ctl_op->u4_min_num_in_bufs = MIN_IN_BUFS;
 if(ps_codec->e_chroma_fmt == IV_YUV_420P)
        ps_ctl_op->u4_min_num_out_bufs = MIN_OUT_BUFS_420;
 else if(ps_codec->e_chroma_fmt == IV_YUV_422ILE)
        ps_ctl_op->u4_min_num_out_bufs = MIN_OUT_BUFS_422ILE;
 else if(ps_codec->e_chroma_fmt == IV_RGB_565)
        ps_ctl_op->u4_min_num_out_bufs = MIN_OUT_BUFS_RGB565;
 else if(ps_codec->e_chroma_fmt == IV_RGBA_8888)
        ps_ctl_op->u4_min_num_out_bufs = MIN_OUT_BUFS_RGBA8888;
 else if((ps_codec->e_chroma_fmt == IV_YUV_420SP_UV)
 || (ps_codec->e_chroma_fmt == IV_YUV_420SP_VU))
        ps_ctl_op->u4_min_num_out_bufs = MIN_OUT_BUFS_420SP;

    ps_ctl_op->u4_num_disp_bufs = 1;

 for(i = 0; i < ps_ctl_op->u4_min_num_in_bufs; i++)
 {
        wd = ALIGN64(ps_codec->i4_wd);
        ht = ALIGN64(ps_codec->i4_ht);

        ps_ctl_op->u4_min_in_buf_size[i] = MAX((wd * ht), MIN_BITSBUF_SIZE);
 }

    wd = 0;
    ht = 0;

 if(ps_codec->i4_sps_done)
 {
 if(0 == ps_codec->i4_share_disp_buf)
 {
            wd = ps_codec->i4_disp_wd;
            ht = ps_codec->i4_disp_ht;

 }
 else
 {
            wd = ps_codec->i4_disp_strd;
            ht = ps_codec->i4_ht + PAD_HT;
 }
 }
 else
 {
 if(1 == ps_codec->i4_share_disp_buf)
 {
            wd = ALIGN32(wd + PAD_WD);
            ht += PAD_HT;
 }
 }

 if(ps_codec->i4_disp_strd > wd)
        wd = ps_codec->i4_disp_strd;

 if(0 == ps_codec->i4_share_disp_buf)
        ps_ctl_op->u4_num_disp_bufs = 1;
 else
 {
 if(ps_codec->i4_sps_done)
 {
 sps_t *ps_sps = (ps_codec->s_parse.ps_sps_base + ps_codec->i4_sps_id);
            WORD32 reorder_pic_cnt, ref_pic_cnt;
            reorder_pic_cnt = 0;
 if(ps_codec->e_frm_out_mode != IVD_DECODE_FRAME_OUT)
                reorder_pic_cnt = ps_sps->ai1_sps_max_num_reorder_pics[ps_sps->i1_sps_max_sub_layers - 1];
            ref_pic_cnt = ps_sps->ai1_sps_max_dec_pic_buffering[ps_sps->i1_sps_max_sub_layers - 1];

            ps_ctl_op->u4_num_disp_bufs = reorder_pic_cnt;

            ps_ctl_op->u4_num_disp_bufs += ref_pic_cnt + 1;
 }
 else
 {
            ps_ctl_op->u4_num_disp_bufs = MAX_REF_CNT;
 }

        ps_ctl_op->u4_num_disp_bufs = MIN(
                        ps_ctl_op->u4_num_disp_bufs, 32);

 }

  
 if(ps_codec->e_chroma_fmt == IV_YUV_420P)
 {
        ps_ctl_op->u4_min_out_buf_size[0] = (wd * ht);
        ps_ctl_op->u4_min_out_buf_size[1] = (wd * ht) >> 2;
        ps_ctl_op->u4_min_out_buf_size[2] = (wd * ht) >> 2;
 }
 else if(ps_codec->e_chroma_fmt == IV_YUV_422ILE)
 {
        ps_ctl_op->u4_min_out_buf_size[0] = (wd * ht) * 2;
        ps_ctl_op->u4_min_out_buf_size[1] =
                        ps_ctl_op->u4_min_out_buf_size[2] = 0;
 }
 else if(ps_codec->e_chroma_fmt == IV_RGB_565)
 {
        ps_ctl_op->u4_min_out_buf_size[0] = (wd * ht) * 2;
        ps_ctl_op->u4_min_out_buf_size[1] =
                        ps_ctl_op->u4_min_out_buf_size[2] = 0;
 }
 else if(ps_codec->e_chroma_fmt == IV_RGBA_8888)
 {
        ps_ctl_op->u4_min_out_buf_size[0] = (wd * ht) * 4;
        ps_ctl_op->u4_min_out_buf_size[1] =
                        ps_ctl_op->u4_min_out_buf_size[2] = 0;
 }
 else if((ps_codec->e_chroma_fmt == IV_YUV_420SP_UV)
 || (ps_codec->e_chroma_fmt == IV_YUV_420SP_VU))
 {
        ps_ctl_op->u4_min_out_buf_size[0] = (wd * ht);
        ps_ctl_op->u4_min_out_buf_size[1] = (wd * ht) >> 1;
        ps_ctl_op->u4_min_out_buf_size[2] = 0;
 }
    ps_codec->i4_num_disp_bufs = ps_ctl_op->u4_num_disp_bufs;

 return IV_SUCCESS;
}
