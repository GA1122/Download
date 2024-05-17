void WebGL2RenderingContextBase::ResetUnpackParameters() {
  WebGLRenderingContextBase::ResetUnpackParameters();

  if (unpack_row_length_)
    ContextGL()->PixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  if (unpack_image_height_)
    ContextGL()->PixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
  if (unpack_skip_pixels_)
    ContextGL()->PixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
  if (unpack_skip_rows_)
    ContextGL()->PixelStorei(GL_UNPACK_SKIP_ROWS, 0);
  if (unpack_skip_images_)
    ContextGL()->PixelStorei(GL_UNPACK_SKIP_IMAGES, 0);
}
