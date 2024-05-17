 bool Framebuffer::HasAlphaMRT() const {
   for (uint32 i = 0; i < manager_->max_draw_buffers_; ++i) {
     if (draw_buffers_[i] != GL_NONE) {
      const Attachment* attachment = GetAttachment(draw_buffers_[i]);
      if (!attachment)
        continue;
      if ((GLES2Util::GetChannelsForFormat(
               attachment->internal_format()) & 0x0008) != 0)
        return true;
    }
  }
  return false;
}
