vpx_image_t *vpx_img_wrap(vpx_image_t *img, vpx_img_fmt_t fmt, unsigned int d_w,
 unsigned int d_h, unsigned int stride_align,
 unsigned char *img_data) {
  
 return img_alloc_helper(img, fmt, d_w, d_h, 1, stride_align, img_data);
}
