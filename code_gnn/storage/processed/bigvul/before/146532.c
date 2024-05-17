void WebGLRenderingContextBase::readPixels(
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    MaybeShared<DOMArrayBufferView> pixels) {
  ReadPixelsHelper(x, y, width, height, format, type, pixels.View(), 0);
}
