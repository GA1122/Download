void WebGL2RenderingContextBase::texImage2D(
    GLenum target,
    GLint level,
    GLint internalformat,
    GLsizei width,
    GLsizei height,
    GLint border,
    GLenum format,
    GLenum type,
    MaybeShared<DOMArrayBufferView> data,
    GLuint src_offset) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texImage2D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  TexImageHelperDOMArrayBufferView(
      kTexImage2D, target, level, internalformat, width, height, 1, border,
      format, type, 0, 0, 0, data.View(), kNullNotReachable, src_offset);
}
