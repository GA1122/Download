WORD32 ih264d_create_pic_buffers(UWORD8 u1_num_of_buf,
 dec_struct_t *ps_dec)
{
 struct pic_buffer_t *ps_pic_buf;
    UWORD8 i;
    UWORD32 u4_luma_size, u4_chroma_size;
    UWORD8 u1_frm = ps_dec->ps_cur_sps->u1_frame_mbs_only_flag;
    WORD32 j;
    UWORD8 *pu1_buf;

    ps_pic_buf = ps_dec->ps_pic_buf_base;
    ih264_disp_mgr_init((disp_mgr_t *)ps_dec->pv_disp_buf_mgr);
    ih264_buf_mgr_init((buf_mgr_t *)ps_dec->pv_pic_buf_mgr);
    u4_luma_size = ps_dec->u2_frm_wd_y * ps_dec->u2_frm_ht_y;
    u4_chroma_size = ps_dec->u2_frm_wd_uv * ps_dec->u2_frm_ht_uv;

 {
 if(ps_dec->u4_share_disp_buf == 1)
 {
  
 if((ps_dec->u1_chroma_format == IV_YUV_420SP_UV)
 || (ps_dec->u1_chroma_format == IV_YUV_420SP_VU)
 || (ps_dec->u1_chroma_format == IV_YUV_420P))
 {
                u4_luma_size = 64;
 }

 if(ps_dec->u1_chroma_format == IV_YUV_420SP_UV)
 {
                u4_chroma_size = 64;
 }

 }
 }

    pu1_buf = ps_dec->pu1_pic_buf_base;

  
 for(i = 0; i < u1_num_of_buf; i++)
 {
        UWORD32 u4_offset;
        WORD32 buf_ret;
        UWORD8 *pu1_luma, *pu1_chroma;
 void *pv_mem_ctxt = ps_dec->pv_mem_ctxt;

        pu1_luma = pu1_buf;
        pu1_buf += ALIGN64(u4_luma_size);
        pu1_chroma = pu1_buf;
        pu1_buf += ALIGN64(u4_chroma_size);

  

 if((0 == ps_dec->u4_share_disp_buf)
 || (NULL == ps_dec->disp_bufs[i].buf[0]))
 {
            UWORD32 pad_len_h, pad_len_v;

            u4_offset = ps_dec->u2_frm_wd_y * (PAD_LEN_Y_V << 1) + PAD_LEN_Y_H;
            ps_pic_buf->pu1_buf1 = (UWORD8 *)(pu1_luma) + u4_offset;

            pad_len_h = MAX(PAD_LEN_UV_H, (PAD_LEN_Y_H >> 1));
            pad_len_v = MAX(PAD_LEN_UV_V, PAD_LEN_Y_V);

            u4_offset = ps_dec->u2_frm_wd_uv * pad_len_v + pad_len_h;

            ps_pic_buf->pu1_buf2 = (UWORD8 *)(pu1_chroma) + u4_offset;
            ps_pic_buf->pu1_buf3 = (UWORD8 *)(NULL) + u4_offset;

 }
 else
 {
            UWORD32 pad_len_h, pad_len_v;
            u4_offset = ps_dec->u2_frm_wd_y * (PAD_LEN_Y_V << 1) + PAD_LEN_Y_H;
            ps_pic_buf->pu1_buf1 = (UWORD8 *)ps_dec->disp_bufs[i].buf[0]
 + u4_offset;

            ps_dec->disp_bufs[i].u4_ofst[0] = u4_offset;

 if(ps_dec->u1_chroma_format == IV_YUV_420P)
 {
                pad_len_h = MAX(PAD_LEN_UV_H * YUV420SP_FACTOR,
 (PAD_LEN_Y_H >> 1));
                pad_len_v = MAX(PAD_LEN_UV_V, PAD_LEN_Y_V);

                u4_offset = ps_dec->u2_frm_wd_uv * pad_len_v + pad_len_h;
                ps_pic_buf->pu1_buf2 = (UWORD8 *)(pu1_chroma) + u4_offset;
                ps_pic_buf->pu1_buf3 = (UWORD8 *)(NULL) + u4_offset;

                ps_dec->disp_bufs[i].u4_ofst[1] = u4_offset;
                ps_dec->disp_bufs[i].u4_ofst[2] = u4_offset;

 }
 else
 {
                pad_len_h = MAX(PAD_LEN_UV_H * YUV420SP_FACTOR,
 (PAD_LEN_Y_H >> 1));
                pad_len_v = MAX(PAD_LEN_UV_V, PAD_LEN_Y_V);

                u4_offset = ps_dec->u2_frm_wd_uv * pad_len_v + pad_len_h;
                ps_pic_buf->pu1_buf2 = (UWORD8 *)(ps_dec->disp_bufs[i].buf[1])
 + u4_offset;
                ps_pic_buf->pu1_buf3 = (UWORD8 *)(ps_dec->disp_bufs[i].buf[1])
 + u4_offset;

                ps_dec->disp_bufs[i].u4_ofst[1] = u4_offset;
                ps_dec->disp_bufs[i].u4_ofst[2] = u4_offset;
 }
 }

        ps_pic_buf->u2_frm_ht_y = ps_dec->u2_frm_ht_y;
        ps_pic_buf->u2_frm_ht_uv = ps_dec->u2_frm_ht_uv;
        ps_pic_buf->u2_frm_wd_y = ps_dec->u2_frm_wd_y;
        ps_pic_buf->u2_frm_wd_uv = ps_dec->u2_frm_wd_uv;

        ps_pic_buf->u1_pic_buf_id = i;

        buf_ret = ih264_buf_mgr_add((buf_mgr_t *)ps_dec->pv_pic_buf_mgr,
                                    ps_pic_buf, i);
 if(0 != buf_ret)
 {
            ps_dec->i4_error_code = ERROR_BUF_MGR;
 return ERROR_BUF_MGR;
 }

        ps_dec->apv_buf_id_pic_buf_map[i] = (void *)ps_pic_buf;
        ps_pic_buf++;
 }

 if(1 == ps_dec->u4_share_disp_buf)
 {
 for(i = 0; i < u1_num_of_buf; i++)
            ps_dec->u4_disp_buf_mapping[i] = 1;
 }
 return OK;
}
