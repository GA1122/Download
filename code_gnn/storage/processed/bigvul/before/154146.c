GLenum GLES2DecoderImpl::GetBoundReadFramebufferInternalFormat() {
  Framebuffer* framebuffer = GetBoundReadFramebuffer();
  if (framebuffer) {
    return framebuffer->GetReadBufferInternalFormat();
  } else {   
    if (back_buffer_read_buffer_ == GL_NONE)
      return 0;
    if (offscreen_target_frame_buffer_.get()) {
      return offscreen_target_color_format_;
    }
    return back_buffer_color_format_;
  }
}
