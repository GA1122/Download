error::Error GLES2DecoderPassthroughImpl::DoRenderbufferStorage(
    GLenum target,
    GLenum internalformat,
    GLsizei width,
    GLsizei height) {
  api()->glRenderbufferStorageEXTFn(target, internalformat, width, height);
  return error::kNoError;
}
