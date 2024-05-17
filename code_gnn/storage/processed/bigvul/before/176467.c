vpx_image_t *vpx_img_alloc(vpx_image_t *img, vpx_img_fmt_t fmt,
 unsigned int d_w, unsigned int d_h,
 unsigned int align) {
 return img_alloc_helper(img, fmt, d_w, d_h, align, align, NULL);
}
