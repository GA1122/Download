void GLES2DecoderImpl::DoDiscardFramebufferEXT(
    GLenum target,
    GLsizei count,
    const volatile GLenum* attachments) {
  if (workarounds().disable_discard_framebuffer)
    return;

  const GLsizei kWidthNotUsed = 1;
  const GLsizei kHeightNotUsed = 1;
  InvalidateFramebufferImpl(
      target, count, attachments, 0, 0, kWidthNotUsed, kHeightNotUsed,
      "glDiscardFramebufferEXT", kFramebufferDiscard);
}
