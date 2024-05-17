void impeg2d_get_frm_buf(yuv_buf_t *ps_frm_buf,UWORD8 *pu1_frm,UWORD32 u4_width,UWORD32 u4_height)
{
   UWORD32 u4_luma_size = u4_width * u4_height;
   UWORD32 u4_chroma_size = (u4_width * u4_height)>>2;

   ps_frm_buf->pu1_y = pu1_frm;
   ps_frm_buf->pu1_u = pu1_frm + u4_luma_size;
   ps_frm_buf->pu1_v = pu1_frm + u4_luma_size + u4_chroma_size;

}
