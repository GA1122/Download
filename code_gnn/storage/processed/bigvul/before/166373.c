int GLES2Util::ElementsPerGroup(int format, int type) {
  switch (type) {
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
    case GL_UNSIGNED_INT_24_8_OES:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_10F_11F_11F_REV:
    case GL_UNSIGNED_INT_5_9_9_9_REV:
    case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
      return 1;
    default:
      break;
  }

  switch (format) {
    case GL_RGB:
    case GL_RGB_INTEGER:
    case GL_SRGB_EXT:
      return 3;
    case GL_LUMINANCE_ALPHA:
    case GL_RG_EXT:
    case GL_RG_INTEGER:
      return 2;
    case GL_RGBA:
    case GL_RGBA_INTEGER:
    case GL_BGRA_EXT:
    case GL_SRGB_ALPHA_EXT:
      return 4;
    case GL_ALPHA:
    case GL_LUMINANCE:
    case GL_DEPTH_COMPONENT:
    case GL_DEPTH_COMPONENT24_OES:
    case GL_DEPTH_COMPONENT32_OES:
    case GL_DEPTH_COMPONENT16:
    case GL_DEPTH24_STENCIL8_OES:
    case GL_DEPTH_STENCIL_OES:
    case GL_RED_EXT:
    case GL_RED_INTEGER:
      return 1;
    default:
      return 0;
  }
}
