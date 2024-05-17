error::Error GLES2DecoderPassthroughImpl::DoCompressedTexImage3D(
    GLenum target,
    GLint level,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLint border,
    GLsizei image_size,
    GLsizei data_size,
    const void* data) {
  CheckErrorCallbackState();
  api()->glCompressedTexImage3DRobustANGLEFn(target, level, internalformat,
                                             width, height, depth, border,
                                             image_size, data_size, data);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  UpdateTextureSizeFromTarget(target);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
