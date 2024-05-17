void WebGLRenderingContextBase::texSubImage2D(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    MaybeShared<DOMArrayBufferView> pixels) {
  TexImageHelperDOMArrayBufferView(kTexSubImage2D, target, level, 0, width,
                                   height, 1, 0, format, type, xoffset, yoffset,
                                   0, pixels.View(), kNullNotAllowed, 0);
}
