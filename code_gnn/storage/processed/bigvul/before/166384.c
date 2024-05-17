void GLES2Util::GetColorFormatComponentSizes(
    uint32_t internal_format, uint32_t type, int* r, int* g, int* b, int* a) {
  DCHECK(r && g && b && a);
  *r = 0;
  *g = 0;
  *b = 0;
  *a = 0;

  switch (internal_format) {
    case GL_LUMINANCE:
      switch (type) {
        case GL_UNSIGNED_BYTE:
          internal_format = GL_R8;
          break;
        case GL_HALF_FLOAT_OES:
          internal_format = GL_R16F;
          break;
        case GL_FLOAT:
          internal_format = GL_R32F;
          return;
        default:
          NOTREACHED();
          break;
      }
      break;
    case GL_LUMINANCE_ALPHA:
      switch (type) {
        case GL_UNSIGNED_BYTE:
          internal_format = GL_RGBA8;
          break;
        case GL_HALF_FLOAT_OES:
          internal_format = GL_RGBA16F;
          break;
        case GL_FLOAT:
          internal_format = GL_RGBA32F;
          return;
        default:
          NOTREACHED();
          break;
      }
      break;
    default:
      internal_format = ConvertToSizedFormat(internal_format, type);
      break;
  }

  switch (internal_format) {
    case GL_ALPHA8_EXT:
      *a = 8;
      break;
    case GL_ALPHA16F_EXT:
      *a = 16;
      break;
    case GL_ALPHA32F_EXT:
      *a = 32;
      break;
    case GL_RGB8_OES:
    case GL_SRGB8:
    case GL_RGB8_SNORM:
    case GL_RGB8UI:
    case GL_RGB8I:
      *r = 8;
      *g = 8;
      *b = 8;
      break;
    case GL_RGB565:
      *r = 5;
      *g = 6;
      *b = 5;
      break;
    case GL_RGB16F:
    case GL_RGB16UI:
    case GL_RGB16I:
      *r = 16;
      *g = 16;
      *b = 16;
      break;
    case GL_RGB32F:
    case GL_RGB32UI:
    case GL_RGB32I:
      *r = 32;
      *g = 32;
      *b = 32;
      break;
    case GL_R11F_G11F_B10F:
      *r = 11;
      *g = 11;
      *b = 10;
      break;
    case GL_RGB9_E5:
      *r = 9;
      *g = 9;
      *b = 9;
      break;
    case GL_BGRA8_EXT:
    case GL_RGBA8:
    case GL_SRGB8_ALPHA8:
    case GL_RGBA8_SNORM:
    case GL_RGBA8UI:
    case GL_RGBA8I:
      *r = 8;
      *g = 8;
      *b = 8;
      *a = 8;
      break;
    case GL_RGBA16F_EXT:
    case GL_RGBA16UI:
    case GL_RGBA16I:
      *r = 16;
      *g = 16;
      *b = 16;
      *a = 16;
      break;
    case GL_RGBA32F_EXT:
    case GL_RGBA32UI:
    case GL_RGBA32I:
      *r = 32;
      *g = 32;
      *b = 32;
      *a = 32;
      break;
    case GL_RGBA4:
      *r = 4;
      *g = 4;
      *b = 4;
      *a = 4;
      break;
    case GL_RGB5_A1:
      *r = 5;
      *g = 5;
      *b = 5;
      *a = 1;
      break;
    case GL_RGB10_A2:
    case GL_RGB10_A2UI:
      *r = 10;
      *g = 10;
      *b = 10;
      *a = 2;
      break;
    case GL_R8:
    case GL_R8_SNORM:
    case GL_R8UI:
    case GL_R8I:
      *r = 8;
      break;
    case GL_R16F:
    case GL_R16UI:
    case GL_R16I:
    case GL_R16_EXT:
      *r = 16;
      break;
    case GL_R32F:
    case GL_R32UI:
    case GL_R32I:
      *r = 32;
      break;
    case GL_RG8:
    case GL_RG8_SNORM:
    case GL_RG8UI:
    case GL_RG8I:
      *r = 8;
      *g = 8;
      break;
    case GL_RG16F:
    case GL_RG16UI:
    case GL_RG16I:
      *r = 16;
      *g = 16;
      break;
    case GL_RG32F:
    case GL_RG32UI:
    case GL_RG32I:
      *r = 32;
      *g = 32;
      break;
    default:
      NOTREACHED();
      break;
  }
}
