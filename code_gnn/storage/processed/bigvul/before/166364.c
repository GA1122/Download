int BytesPerElement(int type) {
  switch (type) {
    case GL_FLOAT_32_UNSIGNED_INT_24_8_REV:
      return 8;
    case GL_FLOAT:
    case GL_UNSIGNED_INT_24_8_OES:
    case GL_UNSIGNED_INT:
    case GL_INT:
    case GL_UNSIGNED_INT_2_10_10_10_REV:
    case GL_UNSIGNED_INT_10F_11F_11F_REV:
    case GL_UNSIGNED_INT_5_9_9_9_REV:
      return 4;
    case GL_HALF_FLOAT:
    case GL_HALF_FLOAT_OES:
    case GL_UNSIGNED_SHORT:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT_5_6_5:
    case GL_UNSIGNED_SHORT_4_4_4_4:
    case GL_UNSIGNED_SHORT_5_5_5_1:
      return 2;
    case GL_UNSIGNED_BYTE:
    case GL_BYTE:
      return 1;
    default:
      return 0;
  }
}
