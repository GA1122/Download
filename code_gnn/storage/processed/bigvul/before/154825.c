error::Error GLES2DecoderPassthroughImpl::DoTexSubImage3D(GLenum target,
                                                          GLint level,
                                                          GLint xoffset,
                                                          GLint yoffset,
                                                          GLint zoffset,
                                                          GLsizei width,
                                                          GLsizei height,
                                                          GLsizei depth,
                                                          GLenum format,
                                                          GLenum type,
                                                          GLsizei image_size,
                                                          const void* pixels) {
  ScopedUnpackStateButAlignmentReset reset_unpack(
      api(), image_size != 0 && feature_info_->gl_version_info().is_es3, true);
  api()->glTexSubImage3DRobustANGLEFn(target, level, xoffset, yoffset, zoffset,
                                      width, height, depth, format, type,
                                      image_size, pixels);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
