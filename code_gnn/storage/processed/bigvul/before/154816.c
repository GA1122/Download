error::Error GLES2DecoderPassthroughImpl::DoTexImage3D(GLenum target,
                                                       GLint level,
                                                       GLint internalformat,
                                                       GLsizei width,
                                                       GLsizei height,
                                                       GLsizei depth,
                                                       GLint border,
                                                       GLenum format,
                                                       GLenum type,
                                                       GLsizei image_size,
                                                       const void* pixels) {
  ScopedUnpackStateButAlignmentReset reset_unpack(
      api(), image_size != 0 && feature_info_->gl_version_info().is_es3, true);

  CheckErrorCallbackState();
  api()->glTexImage3DRobustANGLEFn(target, level, internalformat, width, height,
                                   depth, border, format, type, image_size,
                                   pixels);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  UpdateTextureSizeFromTarget(target);

  ExitCommandProcessingEarly();

  return error::kNoError;
}
