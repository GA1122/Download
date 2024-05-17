GLint GLES2Util::GetColorEncodingFromInternalFormat(uint32_t internalformat) {
  switch (internalformat) {
    case GL_SRGB_EXT:
    case GL_SRGB_ALPHA_EXT:
    case GL_SRGB8:
    case GL_SRGB8_ALPHA8:
      return GL_SRGB;
    default:
      return GL_LINEAR;
  }
}
