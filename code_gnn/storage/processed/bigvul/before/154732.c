error::Error GLES2DecoderPassthroughImpl::DoInvalidateSubFramebuffer(
    GLenum target,
    GLsizei count,
    const volatile GLenum* attachments,
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height) {
  if (count < 0) {
    InsertError(GL_INVALID_VALUE, "count cannot be negative.");
    return error::kNoError;
  }

  std::vector<GLenum> attachments_copy(attachments, attachments + count);
  if (IsEmulatedFramebufferBound(target)) {
    if (!ModifyAttachmentsForEmulatedFramebuffer(&attachments_copy)) {
      InsertError(GL_INVALID_OPERATION, "Invalid attachment.");
      return error::kNoError;
    }
  }
  api()->glInvalidateSubFramebufferFn(target, count, attachments_copy.data(), x,
                                      y, width, height);
  return error::kNoError;
}
