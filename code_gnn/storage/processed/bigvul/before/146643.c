    ScopedRGBEmulationForBlitFramebuffer(DrawingBuffer* drawing_buffer)
    : drawing_buffer_(drawing_buffer) {
  doing_work_ = drawing_buffer->SetupRGBEmulationForBlitFramebuffer();
}
