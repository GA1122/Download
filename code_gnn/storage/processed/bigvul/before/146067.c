void WebGL2RenderingContextBase::compressedTexImage2D(
    GLenum target,
    GLint level,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    GLint border,
    MaybeShared<DOMArrayBufferView> data) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "compressedTexImage2D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  WebGLRenderingContextBase::compressedTexImage2D(target, level, internalformat,
                                                  width, height, border, data);
}
