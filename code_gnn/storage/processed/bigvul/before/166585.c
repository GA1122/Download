    DrawingBufferClientRestorePixelUnpackBufferBinding() {
  if (destruction_in_progress_)
    return;
  if (!ContextGL())
    return;
  ContextGL()->BindBuffer(GL_PIXEL_UNPACK_BUFFER,
                          ObjectOrZero(bound_pixel_unpack_buffer_.Get()));
}
