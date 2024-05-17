uint8_t GLES2Util::StencilBitsPerPixel(int format) {
  switch (format) {
    case GL_STENCIL_INDEX8:
    case GL_DEPTH24_STENCIL8_OES:
      return 8;
    default:
      return 0;
  }
}
