uint32_t GLES2Util::ConvertToSizedFormat(uint32_t format, uint32_t type) {
  switch (format) {
    case GL_RGB:
      switch (type) {
        case GL_UNSIGNED_BYTE:
          return GL_RGB8;
        case GL_UNSIGNED_SHORT_5_6_5:
          return GL_RGB565;
        case GL_HALF_FLOAT_OES:
          return GL_RGB16F;
        case GL_FLOAT:
          return GL_RGB32F;
        default:
          NOTREACHED();
          break;
      }
      break;
    case GL_RGBA:
      switch (type) {
        case GL_UNSIGNED_BYTE:
          return GL_RGBA8;
        case GL_UNSIGNED_SHORT_4_4_4_4:
          return GL_RGBA4;
        case GL_UNSIGNED_SHORT_5_5_5_1:
          return GL_RGB5_A1;
        case GL_HALF_FLOAT_OES:
          return GL_RGBA16F;
        case GL_FLOAT:
          return GL_RGBA32F;
        default:
          NOTREACHED();
          break;
      }
      break;
    case GL_ALPHA:
      switch (type) {
        case GL_UNSIGNED_BYTE:
          return GL_ALPHA8_EXT;
        case GL_HALF_FLOAT_OES:
          return GL_ALPHA16F_EXT;
        case GL_FLOAT:
          return GL_ALPHA32F_EXT;
        default:
          NOTREACHED();
          break;
      }
      break;
    case GL_RED:
      switch (type) {
        case GL_UNSIGNED_BYTE:
          return GL_R8;
        case GL_HALF_FLOAT_OES:
          return GL_R16F;
        case GL_FLOAT:
          return GL_R32F;
        case GL_UNSIGNED_SHORT:
          return GL_R16_EXT;
        default:
          NOTREACHED();
          break;
      }
      break;
    case GL_RG:
      switch (type) {
        case GL_UNSIGNED_BYTE:
          return GL_RG8;
        case GL_HALF_FLOAT_OES:
          return GL_RG16F;
        case GL_FLOAT:
          return GL_RG32F;
        default:
          NOTREACHED();
          break;
      }
      break;
    case GL_SRGB_EXT:
      switch (type) {
        case GL_UNSIGNED_BYTE:
          return GL_SRGB8;
        default:
          NOTREACHED();
          break;
      }
      break;
    case GL_SRGB_ALPHA_EXT:
      switch (type) {
        case GL_UNSIGNED_BYTE:
          return GL_SRGB8_ALPHA8;
        default:
          NOTREACHED();
          break;
      }
      break;
    case GL_BGRA_EXT:
      switch (type) {
        case GL_UNSIGNED_BYTE:
          return GL_BGRA8_EXT;
        default:
          NOTREACHED();
          break;
      }
      break;
    default:
      break;
  }

  return format;
}
