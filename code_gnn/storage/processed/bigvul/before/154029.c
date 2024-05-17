void GLES2DecoderImpl::DoInvalidateSubFramebuffer(
    GLenum target,
    GLsizei count,
    const volatile GLenum* attachments,
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height) {
  InvalidateFramebufferImpl(
      target, count, attachments, x, y, width, height,
      "glInvalidateSubFramebuffer", kFramebufferInvalidateSub);
}
