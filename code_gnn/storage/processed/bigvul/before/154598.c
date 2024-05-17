error::Error GLES2DecoderPassthroughImpl::DoCompressedTexSubImage2D(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLsizei image_size,
    GLsizei data_size,
    const void* data) {
  api()->glCompressedTexSubImage2DRobustANGLEFn(target, level, xoffset, yoffset,
                                                width, height, format,
                                                image_size, data_size, data);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
