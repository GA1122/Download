static void yuvconfig2image(vpx_image_t *img,
 const YV12_BUFFER_CONFIG  *yv12,
 void *user_priv)
{
  
    img->fmt = VPX_IMG_FMT_I420;
    img->w = yv12->y_stride;
    img->h = (yv12->y_height + 2 * VP8BORDERINPIXELS + 15) & ~15;
    img->d_w = yv12->y_width;
    img->d_h = yv12->y_height;
    img->x_chroma_shift = 1;
    img->y_chroma_shift = 1;
    img->planes[VPX_PLANE_Y] = yv12->y_buffer;
    img->planes[VPX_PLANE_U] = yv12->u_buffer;
    img->planes[VPX_PLANE_V] = yv12->v_buffer;
    img->planes[VPX_PLANE_ALPHA] = NULL;
    img->stride[VPX_PLANE_Y] = yv12->y_stride;
    img->stride[VPX_PLANE_U] = yv12->uv_stride;
    img->stride[VPX_PLANE_V] = yv12->uv_stride;
    img->stride[VPX_PLANE_ALPHA] = yv12->y_stride;
    img->bit_depth = 8;
    img->bps = 12;
    img->user_priv = user_priv;
    img->img_data = yv12->buffer_alloc;
    img->img_data_owner = 0;
    img->self_allocd = 0;
}
