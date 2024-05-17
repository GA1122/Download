error::Error GLES2DecoderPassthroughImpl::DoTexStorage2DEXT(
    GLenum target,
    GLsizei levels,
    GLenum internalFormat,
    GLsizei width,
    GLsizei height) {
  CheckErrorCallbackState();
  api()->glTexStorage2DEXTFn(target, levels, internalFormat, width, height);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }
  UpdateTextureSizeFromTarget(target);
  return error::kNoError;
}
