static GLenum ExtractFormatFromStorageFormat(GLenum internalformat) {
  switch (internalformat) {
    case GL_RGB565:
      return GL_RGB;
    case GL_RGBA4:
      return GL_RGBA;
    case GL_RGB5_A1:
      return GL_RGBA;
    case GL_RGB8_OES:
      return GL_RGB;
    case GL_RGBA8_OES:
      return GL_RGBA;
    case GL_LUMINANCE8_ALPHA8_EXT:
      return GL_LUMINANCE_ALPHA;
    case GL_LUMINANCE8_EXT:
      return GL_LUMINANCE;
    case GL_ALPHA8_EXT:
      return GL_ALPHA;
    case GL_RGBA32F_EXT:
      return GL_RGBA;
    case GL_RGB32F_EXT:
      return GL_RGB;
    case GL_ALPHA32F_EXT:
      return GL_ALPHA;
    case GL_LUMINANCE32F_EXT:
      return GL_LUMINANCE;
    case GL_LUMINANCE_ALPHA32F_EXT:
      return GL_LUMINANCE_ALPHA;
    case GL_RGBA16F_EXT:
      return GL_RGBA;
    case GL_RGB16F_EXT:
      return GL_RGB;
    case GL_ALPHA16F_EXT:
      return GL_ALPHA;
    case GL_LUMINANCE16F_EXT:
      return GL_LUMINANCE;
    case GL_LUMINANCE_ALPHA16F_EXT:
      return GL_LUMINANCE_ALPHA;
    case GL_BGRA8_EXT:
      return GL_BGRA_EXT;
    default:
      return GL_NONE;
  }
}
