error::Error GLES2DecoderPassthroughImpl::DoTexSubImage2D(GLenum target,
                                                          GLint level,
                                                          GLint xoffset,
                                                          GLint yoffset,
                                                          GLsizei width,
                                                          GLsizei height,
                                                          GLenum format,
                                                          GLenum type,
                                                          GLsizei image_size,
                                                          const void* pixels) {
  ScopedUnpackStateButAlignmentReset reset_unpack(
      api(), image_size != 0 && feature_info_->gl_version_info().is_es3, false);
  api()->glTexSubImage2DRobustANGLEFn(target, level, xoffset, yoffset, width,
                                      height, format, type, image_size, pixels);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
