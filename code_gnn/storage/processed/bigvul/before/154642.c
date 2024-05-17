error::Error GLES2DecoderPassthroughImpl::DoDrawBuffersEXT(
    GLsizei count,
    const volatile GLenum* bufs) {
  if (count < 0) {
    InsertError(GL_INVALID_VALUE, "count cannot be negative.");
    return error::kNoError;
  }
  std::vector<GLenum> bufs_copy(bufs, bufs + count);
  api()->glDrawBuffersARBFn(count, bufs_copy.data());
  return error::kNoError;
}
