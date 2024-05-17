error::Error GLES2DecoderPassthroughImpl::DoCompressedTexImage2D(
    GLenum target,
    GLint level,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    GLint border,
    GLsizei image_size,
    GLsizei data_size,
    const void* data) {
  CheckErrorCallbackState();
  api()->glCompressedTexImage2DRobustANGLEFn(target, level, internalformat,
                                             width, height, border, image_size,
                                             data_size, data);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  UpdateTextureSizeFromTarget(target);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
