void GLES2DecoderImpl::DoInvalidateFramebuffer(
    GLenum target,
    GLsizei count,
    const volatile GLenum* attachments) {
  const GLsizei kWidthNotUsed = 1;
  const GLsizei kHeightNotUsed = 1;
  InvalidateFramebufferImpl(
      target, count, attachments, 0, 0, kWidthNotUsed, kHeightNotUsed,
      "glInvalidateFramebuffer", kFramebufferInvalidate);
}
