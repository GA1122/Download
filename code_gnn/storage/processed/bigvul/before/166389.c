uint32_t GLES2Util::GetGLReadPixelsImplementationType(uint32_t internal_format,
                                                      uint32_t texture_type) {
  switch (internal_format) {
    case GL_R16UI:
    case GL_RG16UI:
    case GL_RGBA16UI:
    case GL_RGB10_A2:
    case GL_RGB10_A2UI:
      return GL_UNSIGNED_SHORT;
    case GL_R32UI:
    case GL_RG32UI:
    case GL_RGBA32UI:
      return GL_UNSIGNED_INT;
    case GL_R8I:
    case GL_RG8I:
    case GL_RGBA8I:
      return GL_BYTE;
    case GL_R16I:
    case GL_RG16I:
    case GL_RGBA16I:
      return GL_SHORT;
    case GL_R32I:
    case GL_RG32I:
    case GL_RGBA32I:
      return GL_INT;
    case GL_R32F:
    case GL_RG32F:
    case GL_RGB32F:
    case GL_RGBA32F:
    case GL_R11F_G11F_B10F:
      return GL_UNSIGNED_BYTE;
    case GL_R16F:
    case GL_RG16F:
    case GL_RGB16F:
    case GL_RGBA16F:
      return GL_HALF_FLOAT;
    default:
      return texture_type;
  }
}
