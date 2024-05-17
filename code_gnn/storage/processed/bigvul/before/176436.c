WORD32 check_app_out_buf_size(dec_struct_t *ps_dec)
{
    UWORD32 au4_min_out_buf_size[IVD_VIDDEC_MAX_IO_BUFFERS];
    UWORD32 u4_min_num_out_bufs, i;
    UWORD32 pic_wd, pic_ht;

 if(0 == ps_dec->u4_share_disp_buf)
 {
        pic_wd = ps_dec->u2_disp_width;
        pic_ht = ps_dec->u2_disp_height;

 }
 else
 {
  
 return (IV_SUCCESS);
 }

 if(ps_dec->u4_app_disp_width > pic_wd)
        pic_wd = ps_dec->u4_app_disp_width;

    u4_min_num_out_bufs = ih264d_get_outbuf_size(pic_wd, pic_ht,
                                                 ps_dec->u1_chroma_format,
 &au4_min_out_buf_size[0]);

 if(ps_dec->ps_out_buffer->u4_num_bufs < u4_min_num_out_bufs)
 return IV_FAIL;

 for(i = 0; i < u4_min_num_out_bufs; i++)
 {
 if(ps_dec->ps_out_buffer->u4_min_out_buf_size[i]
 < au4_min_out_buf_size[i])
 return (IV_FAIL);
 }

 return (IV_SUCCESS);
}
