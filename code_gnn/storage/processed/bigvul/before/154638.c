error::Error GLES2DecoderPassthroughImpl::DoDiscardFramebufferEXT(
    GLenum target,
    GLsizei count,
    const volatile GLenum* attachments) {
  if (count < 0) {
    InsertError(GL_INVALID_VALUE, "count cannot be negative.");
    return error::kNoError;
  }
  std::vector<GLenum> attachments_copy(attachments, attachments + count);

  if (feature_info_->gl_version_info().is_es3) {
    api()->glInvalidateFramebufferFn(target, count, attachments_copy.data());
  } else {
    api()->glDiscardFramebufferEXTFn(target, count, attachments_copy.data());
  }

  return error::kNoError;
}
