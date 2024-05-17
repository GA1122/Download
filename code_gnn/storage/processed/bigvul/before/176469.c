void vpx_img_free(vpx_image_t *img) {
 if (img) {
 if (img->img_data && img->img_data_owner) vpx_free(img->img_data);

 if (img->self_allocd) free(img);
 }
}
