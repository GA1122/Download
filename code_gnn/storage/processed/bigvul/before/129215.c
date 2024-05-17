bool GLES2DecoderImpl::BoundFramebufferHasColorAttachmentWithAlpha(
    bool all_draw_buffers) {
  Framebuffer* framebuffer =
      GetFramebufferInfoForTarget(GL_DRAW_FRAMEBUFFER_EXT);
  if (!all_draw_buffers || !framebuffer) {
    return (GLES2Util::GetChannelsForFormat(
        GetBoundDrawFrameBufferInternalFormat()) & 0x0008) != 0;
  }
  return framebuffer->HasAlphaMRT();
}
