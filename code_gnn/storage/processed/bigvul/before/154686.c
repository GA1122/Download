error::Error GLES2DecoderPassthroughImpl::DoGetFramebufferAttachmentParameteriv(
    GLenum target,
    GLenum attachment,
    GLenum pname,
    GLsizei bufsize,
    GLsizei* length,
    GLint* params) {
  GLenum updated_attachment = attachment;
  if (IsEmulatedFramebufferBound(target)) {
    if (!ModifyAttachmentForEmulatedFramebuffer(&updated_attachment)) {
      InsertError(GL_INVALID_OPERATION, "Invalid attachment.");
      *length = 0;
      return error::kNoError;
    }

    switch (pname) {
      case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME:
      case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL:
      case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE:
      case GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER:
        InsertError(GL_INVALID_ENUM, "Invalid parameter name.");
        *length = 0;
        return error::kNoError;
    }
  }

  CheckErrorCallbackState();

  GLint* scratch_params = GetTypedScratchMemory<GLint>(bufsize);
  api()->glGetFramebufferAttachmentParameterivRobustANGLEFn(
      target, updated_attachment, pname, bufsize, length, scratch_params);

  if (CheckErrorCallbackState()) {
    DCHECK(*length == 0);
    return error::kNoError;
  }

  error::Error error = PatchGetFramebufferAttachmentParameter(
      target, updated_attachment, pname, *length, scratch_params);
  if (error != error::kNoError) {
    *length = 0;
    return error;
  }

  DCHECK(*length < bufsize);
  std::copy(scratch_params, scratch_params + *length, params);

  return error::kNoError;
}
