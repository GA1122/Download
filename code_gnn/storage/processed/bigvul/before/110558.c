static inline GLenum GetTexInternalFormat(GLenum internal_format) {
  if (gfx::GetGLImplementation() != gfx::kGLImplementationEGLGLES2) {
    if (internal_format == GL_BGRA_EXT || internal_format == GL_BGRA8_EXT)
      return GL_RGBA8;
  }
  return internal_format;
}
