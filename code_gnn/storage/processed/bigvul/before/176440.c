UWORD32 ih264d_get_outbuf_size(WORD32 pic_wd,
                               UWORD32 pic_ht,
                               UWORD8 u1_chroma_format,
                               UWORD32 *p_buf_size)
{
    UWORD32 u4_min_num_out_bufs = 0;

 if(u1_chroma_format == IV_YUV_420P)
        u4_min_num_out_bufs = MIN_OUT_BUFS_420;
 else if(u1_chroma_format == IV_YUV_422ILE)
        u4_min_num_out_bufs = MIN_OUT_BUFS_422ILE;
 else if(u1_chroma_format == IV_RGB_565)
        u4_min_num_out_bufs = MIN_OUT_BUFS_RGB565;
 else if((u1_chroma_format == IV_YUV_420SP_UV)
 || (u1_chroma_format == IV_YUV_420SP_VU))
        u4_min_num_out_bufs = MIN_OUT_BUFS_420SP;

 if(u1_chroma_format == IV_YUV_420P)
 {
        p_buf_size[0] = (pic_wd * pic_ht);
        p_buf_size[1] = (pic_wd * pic_ht) >> 2;
        p_buf_size[2] = (pic_wd * pic_ht) >> 2;
 }
 else if(u1_chroma_format == IV_YUV_422ILE)
 {
        p_buf_size[0] = (pic_wd * pic_ht) * 2;
        p_buf_size[1] = p_buf_size[2] = 0;
 }
 else if(u1_chroma_format == IV_RGB_565)
 {
        p_buf_size[0] = (pic_wd * pic_ht) * 2;
        p_buf_size[1] = p_buf_size[2] = 0;
 }
 else if((u1_chroma_format == IV_YUV_420SP_UV)
 || (u1_chroma_format == IV_YUV_420SP_VU))
 {
        p_buf_size[0] = (pic_wd * pic_ht);
        p_buf_size[1] = (pic_wd * pic_ht) >> 1;
        p_buf_size[2] = 0;
 }

 return u4_min_num_out_bufs;
}
