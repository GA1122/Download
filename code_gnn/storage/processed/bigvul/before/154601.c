error::Error GLES2DecoderPassthroughImpl::DoCopySubTextureCHROMIUM(
    GLuint source_id,
    GLint source_level,
    GLenum dest_target,
    GLuint dest_id,
    GLint dest_level,
    GLint xoffset,
    GLint yoffset,
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height,
    GLboolean unpack_flip_y,
    GLboolean unpack_premultiply_alpha,
    GLboolean unpack_unmultiply_alpha) {
  BindPendingImageForClientIDIfNeeded(source_id);
  api()->glCopySubTextureCHROMIUMFn(
      GetTextureServiceID(api(), source_id, resources_, false), source_level,
      dest_target, GetTextureServiceID(api(), dest_id, resources_, false),
      dest_level, xoffset, yoffset, x, y, width, height, unpack_flip_y,
      unpack_premultiply_alpha, unpack_unmultiply_alpha);
  return error::kNoError;
}
