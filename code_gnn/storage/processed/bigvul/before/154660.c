error::Error GLES2DecoderPassthroughImpl::DoFramebufferTextureMultiviewOVR(
    GLenum target,
    GLenum attachment,
    GLuint texture,
    GLint level,
    GLint base_view_index,
    GLsizei num_views) {
  if (IsEmulatedFramebufferBound(target)) {
    InsertError(GL_INVALID_OPERATION,
                "Cannot change the attachments of the default framebuffer.");
    return error::kNoError;
  }
  api()->glFramebufferTextureMultiviewOVRFn(
      target, attachment,
      GetTextureServiceID(api(), texture, resources_, false), level,
      base_view_index, num_views);
  return error::kNoError;
}
