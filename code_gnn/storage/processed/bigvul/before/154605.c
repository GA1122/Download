error::Error GLES2DecoderPassthroughImpl::DoCopyTextureCHROMIUM(
    GLuint source_id,
    GLint source_level,
    GLenum dest_target,
    GLuint dest_id,
    GLint dest_level,
    GLint internalformat,
    GLenum dest_type,
    GLboolean unpack_flip_y,
    GLboolean unpack_premultiply_alpha,
    GLboolean unpack_unmultiply_alpha) {
  BindPendingImageForClientIDIfNeeded(source_id);
  api()->glCopyTextureCHROMIUMFn(
      GetTextureServiceID(api(), source_id, resources_, false), source_level,
      dest_target, GetTextureServiceID(api(), dest_id, resources_, false),
      dest_level, internalformat, dest_type, unpack_flip_y,
      unpack_premultiply_alpha, unpack_unmultiply_alpha);

  UpdateTextureSizeFromClientID(dest_id);

  return error::kNoError;
}
