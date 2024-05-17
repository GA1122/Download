    DrawingBufferClientRestorePixelPackBufferBinding() {
  if (destruction_in_progress_)
    return;
  if (!ContextGL())
    return;
  ContextGL()->BindBuffer(GL_PIXEL_PACK_BUFFER,
                          ObjectOrZero(bound_pixel_pack_buffer_.Get()));
}
