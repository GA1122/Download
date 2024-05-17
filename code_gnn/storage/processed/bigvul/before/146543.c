void WebGLRenderingContextBase::texImage2D(
    GLenum target,
    GLint level,
    GLint internalformat,
    GLsizei width,
    GLsizei height,
    GLint border,
    GLenum format,
    GLenum type,
    MaybeShared<DOMArrayBufferView> pixels) {
  TexImageHelperDOMArrayBufferView(kTexImage2D, target, level, internalformat,
                                   width, height, 1, border, format, type, 0, 0,
                                   0, pixels.View(), kNullAllowed, 0);
}
