void GLES2DecoderImpl::DoCopySubTextureCHROMIUM(
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
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoCopySubTextureCHROMIUM");
  static const char kFunctionName[] = "glCopySubTextureCHROMIUM";
  CopySubTextureHelper(kFunctionName, source_id, source_level, dest_target,
                       dest_id, dest_level, xoffset, yoffset, x, y, width,
                       height, unpack_flip_y, unpack_premultiply_alpha,
                       unpack_unmultiply_alpha, GL_FALSE  );
}
