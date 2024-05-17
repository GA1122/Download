error::Error GLES2DecoderPassthroughImpl::DoTexImage2D(GLenum target,
                                                       GLint level,
                                                       GLint internalformat,
                                                       GLsizei width,
                                                       GLsizei height,
                                                       GLint border,
                                                       GLenum format,
                                                       GLenum type,
                                                       GLsizei image_size,
                                                       const void* pixels) {
  ScopedUnpackStateButAlignmentReset reset_unpack(
      api(), image_size != 0 && feature_info_->gl_version_info().is_es3, false);

  CheckErrorCallbackState();
  api()->glTexImage2DRobustANGLEFn(target, level, internalformat, width, height,
                                   border, format, type, image_size, pixels);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  UpdateTextureSizeFromTarget(target);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
