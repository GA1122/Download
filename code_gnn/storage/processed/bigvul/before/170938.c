static vpx_codec_err_t image2yuvconfig(const vpx_image_t *img,
                                       YV12_BUFFER_CONFIG  *yv12)
{
 const int y_w = img->d_w;
 const int y_h = img->d_h;
 const int uv_w = (img->d_w + 1) / 2;
 const int uv_h = (img->d_h + 1) / 2;
 vpx_codec_err_t        res = VPX_CODEC_OK;
    yv12->y_buffer = img->planes[VPX_PLANE_Y];
    yv12->u_buffer = img->planes[VPX_PLANE_U];
    yv12->v_buffer = img->planes[VPX_PLANE_V];

    yv12->y_crop_width  = y_w;
    yv12->y_crop_height = y_h;
    yv12->y_width  = y_w;
    yv12->y_height = y_h;
    yv12->uv_crop_width = uv_w;
    yv12->uv_crop_height = uv_h;
    yv12->uv_width = uv_w;
    yv12->uv_height = uv_h;

    yv12->y_stride = img->stride[VPX_PLANE_Y];
    yv12->uv_stride = img->stride[VPX_PLANE_U];

    yv12->border  = (img->stride[VPX_PLANE_Y] - img->d_w) / 2;
 return res;
}
