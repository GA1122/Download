error::Error GLES2DecoderPassthroughImpl::DoFramebufferTextureLayer(
    GLenum target,
    GLenum attachment,
    GLuint texture,
    GLint level,
    GLint layer) {
  if (IsEmulatedFramebufferBound(target)) {
    InsertError(GL_INVALID_OPERATION,
                "Cannot change the attachments of the default framebuffer.");
    return error::kNoError;
  }
  api()->glFramebufferTextureLayerFn(
      target, attachment,
      GetTextureServiceID(api(), texture, resources_, false), level, layer);
  return error::kNoError;
}
