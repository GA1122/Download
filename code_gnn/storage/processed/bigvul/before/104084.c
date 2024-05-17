GLenum GLES2DecoderImpl::GetBoundReadFrameBufferInternalFormat() {
  if (bound_read_framebuffer_ != 0) {
    return bound_read_framebuffer_->GetColorAttachmentFormat();
  } else if (offscreen_target_frame_buffer_.get()) {
    return offscreen_target_color_format_;
  } else {
    return back_buffer_color_format_;
  }
}
