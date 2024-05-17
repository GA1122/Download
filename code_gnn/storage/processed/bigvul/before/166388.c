uint32_t GLES2Util::GetGLReadPixelsImplementationFormat(
    uint32_t internal_format,
    uint32_t texture_type,
    bool supports_bgra) {
  switch (internal_format) {
    case GL_R8:
    case GL_R16F:
    case GL_R32F:
      return GL_RED;
    case GL_R8UI:
    case GL_R8I:
    case GL_R16UI:
    case GL_R16I:
    case GL_R32UI:
    case GL_R32I:
      return GL_RED_INTEGER;
    case GL_RG8:
    case GL_RG16F:
    case GL_RG32F:
      return GL_RG;
    case GL_RG8UI:
    case GL_RG8I:
    case GL_RG16UI:
    case GL_RG16I:
    case GL_RG32UI:
    case GL_RG32I:
      return GL_RG_INTEGER;
    case GL_RGB:
    case GL_RGB8:
    case GL_RGB565:
    case GL_R11F_G11F_B10F:
    case GL_RGB16F:
    case GL_RGB32F:
      return GL_RGB;
    case GL_RGBA8UI:
    case GL_RGBA8I:
    case GL_RGB10_A2UI:
    case GL_RGBA16UI:
    case GL_RGBA16I:
    case GL_RGBA32UI:
    case GL_RGBA32I:
      return GL_RGBA_INTEGER;
    case GL_BGRA_EXT:
    case GL_BGRA8_EXT:
      if (texture_type == GL_UNSIGNED_BYTE && supports_bgra)
        return GL_BGRA_EXT;
      else
        return GL_RGBA;
    default:
      return GL_RGBA;
  }
}
