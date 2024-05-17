GLenum GLES2DecoderImpl::GetBoundDrawFrameBufferInternalFormat() {
  if (bound_draw_framebuffer_ != 0) {
    return bound_draw_framebuffer_->GetColorAttachmentFormat();
  } else if (offscreen_target_frame_buffer_.get()) {
    return offscreen_target_color_format_;
  } else {
    return back_buffer_color_format_;
  }
}
