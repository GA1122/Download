error::Error GLES2DecoderPassthroughImpl::DoCompressedTexSubImage3D(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLint zoffset,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLenum format,
    GLsizei image_size,
    GLsizei data_size,
    const void* data) {
  api()->glCompressedTexSubImage3DRobustANGLEFn(
      target, level, xoffset, yoffset, zoffset, width, height, depth, format,
      image_size, data_size, data);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
