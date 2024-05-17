    DrawingBufferClientRestorePixelPackParameters() {
  if (!ContextGL())
    return;

  ContextGL()->PixelStorei(GL_PACK_ROW_LENGTH, pack_row_length_);
  ContextGL()->PixelStorei(GL_PACK_SKIP_ROWS, pack_skip_rows_);
  ContextGL()->PixelStorei(GL_PACK_SKIP_PIXELS, pack_skip_pixels_);

  WebGLRenderingContextBase::DrawingBufferClientRestorePixelPackParameters();
}
