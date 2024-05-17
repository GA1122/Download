gfx::Size GLES2DecoderImpl::GetBoundReadFrameBufferSize() {
  if (bound_read_framebuffer_ != 0) {
    const FramebufferManager::FramebufferInfo::Attachment* attachment =
        bound_read_framebuffer_->GetAttachment(GL_COLOR_ATTACHMENT0);
    if (attachment) {
      return gfx::Size(attachment->width(), attachment->height());
    }
    return gfx::Size(0, 0);
  } else if (offscreen_target_frame_buffer_.get()) {
    return offscreen_size_;
  } else {
    return surface_->GetSize();
  }
}
