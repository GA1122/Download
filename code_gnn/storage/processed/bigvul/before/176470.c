int vpx_img_set_rect(vpx_image_t *img, unsigned int x, unsigned int y,
 unsigned int w, unsigned int h) {
 unsigned char *data;

 if (x + w <= img->w && y + h <= img->h) {
    img->d_w = w;
    img->d_h = h;

  
 if (!(img->fmt & VPX_IMG_FMT_PLANAR)) {
      img->planes[VPX_PLANE_PACKED] =
          img->img_data + x * img->bps / 8 + y * img->stride[VPX_PLANE_PACKED];
 } else {
 const int bytes_per_sample =
 (img->fmt & VPX_IMG_FMT_HIGHBITDEPTH) ? 2 : 1;
      data = img->img_data;

 if (img->fmt & VPX_IMG_FMT_HAS_ALPHA) {
        img->planes[VPX_PLANE_ALPHA] =
            data + x * bytes_per_sample + y * img->stride[VPX_PLANE_ALPHA];
        data += img->h * img->stride[VPX_PLANE_ALPHA];
 }

      img->planes[VPX_PLANE_Y] =
          data + x * bytes_per_sample + y * img->stride[VPX_PLANE_Y];
      data += img->h * img->stride[VPX_PLANE_Y];

 if (!(img->fmt & VPX_IMG_FMT_UV_FLIP)) {
        img->planes[VPX_PLANE_U] =
            data + (x >> img->x_chroma_shift) * bytes_per_sample +
 (y >> img->y_chroma_shift) * img->stride[VPX_PLANE_U];
        data += (img->h >> img->y_chroma_shift) * img->stride[VPX_PLANE_U];
        img->planes[VPX_PLANE_V] =
            data + (x >> img->x_chroma_shift) * bytes_per_sample +
 (y >> img->y_chroma_shift) * img->stride[VPX_PLANE_V];
 } else {
        img->planes[VPX_PLANE_V] =
            data + (x >> img->x_chroma_shift) * bytes_per_sample +
 (y >> img->y_chroma_shift) * img->stride[VPX_PLANE_V];
        data += (img->h >> img->y_chroma_shift) * img->stride[VPX_PLANE_V];
        img->planes[VPX_PLANE_U] =
            data + (x >> img->x_chroma_shift) * bytes_per_sample +
 (y >> img->y_chroma_shift) * img->stride[VPX_PLANE_U];
 }
 }
 return 0;
 }
 return -1;
}
