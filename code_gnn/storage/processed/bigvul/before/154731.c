error::Error GLES2DecoderPassthroughImpl::DoInvalidateFramebuffer(
    GLenum target,
    GLsizei count,
    const volatile GLenum* attachments) {
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
  api()->glInvalidateFramebufferFn(target, count, attachments_copy.data());
  return error::kNoError;
}
