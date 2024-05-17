 jbig2_image_resize(Jbig2Ctx *ctx, Jbig2Image *image, int width, int height)
// jbig2_image_resize(Jbig2Ctx *ctx, Jbig2Image *image, uint32_t width, uint32_t height)
  {
      if (width == image->width) {
           
         int64_t check = ((int64_t) image->stride) * ((int64_t) height);
 
         if (check != (int)check) {
             jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "integer multiplication overflow during resize stride(%d)*height(%d)", image->stride, height);
             return NULL;
         }
          
         image->data = jbig2_renew(ctx, image->data, uint8_t, (int)check);
         if (image->data == NULL) {
             jbig2_error(ctx, JBIG2_SEVERITY_FATAL, -1, "could not resize image buffer!");
             return NULL;
         }
         if (height > image->height) {
             memset(image->data + image->height * image->stride, 0, (height - image->height) * image->stride);
         }
         image->height = height;
 
     } else {
          
         jbig2_error(ctx, JBIG2_SEVERITY_WARNING, -1, "jbig2_image_resize called with a different width (NYI)");
     }
 
     return NULL;
 }