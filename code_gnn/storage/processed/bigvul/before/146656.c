    ~ScopedRGBEmulationForBlitFramebuffer() {
  if (doing_work_) {
    drawing_buffer_->CleanupRGBEmulationForBlitFramebuffer();
  }
}
