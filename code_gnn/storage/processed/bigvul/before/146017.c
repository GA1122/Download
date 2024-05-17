void WebGL2RenderingContextBase::RestoreUnpackParameters() {
  WebGLRenderingContextBase::RestoreUnpackParameters();

  if (unpack_row_length_)
    ContextGL()->PixelStorei(GL_UNPACK_ROW_LENGTH, unpack_row_length_);
  if (unpack_image_height_)
    ContextGL()->PixelStorei(GL_UNPACK_IMAGE_HEIGHT, unpack_image_height_);
  if (unpack_skip_pixels_)
    ContextGL()->PixelStorei(GL_UNPACK_SKIP_PIXELS, unpack_skip_pixels_);
  if (unpack_skip_rows_)
    ContextGL()->PixelStorei(GL_UNPACK_SKIP_ROWS, unpack_skip_rows_);
  if (unpack_skip_images_)
    ContextGL()->PixelStorei(GL_UNPACK_SKIP_IMAGES, unpack_skip_images_);
}
