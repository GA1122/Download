error::Error GLES2DecoderImpl::HandleResizeCHROMIUM(
    uint32 immediate_data_size, const gles2::ResizeCHROMIUM& c) {
  GLuint width = static_cast<GLuint>(c.width);
  GLuint height = static_cast<GLuint>(c.height);
  TRACE_EVENT2("gpu", "glResizeChromium", "width", width, "height", height);
#if defined(OS_POSIX) && !defined(OS_MACOSX) && !defined(TOUCH_UI)
  glFinish();
#endif
  bool is_offscreen = !!offscreen_target_frame_buffer_.get();
  if (is_offscreen) {
    if (!ResizeOffscreenFrameBuffer(gfx::Size(width, height)))
      return error::kLostContext;
  }

  if (resize_callback_.get()) {
    resize_callback_->Run(gfx::Size(width, height));
#if defined(OS_MACOSX)
    if (!context_->MakeCurrent(surface_.get()))
      return error::kLostContext;
#endif
  }

  return error::kNoError;
}
