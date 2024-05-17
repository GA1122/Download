error::Error GLES2DecoderPassthroughImpl::DoFramebufferRenderbuffer(
    GLenum target,
    GLenum attachment,
    GLenum renderbuffertarget,
    GLuint renderbuffer) {
  if (IsEmulatedFramebufferBound(target)) {
    InsertError(GL_INVALID_OPERATION,
                "Cannot change the attachments of the default framebuffer.");
    return error::kNoError;
  }
  api()->glFramebufferRenderbufferEXTFn(
      target, attachment, renderbuffertarget,
      GetRenderbufferServiceID(api(), renderbuffer, resources_, false));
  return error::kNoError;
}
