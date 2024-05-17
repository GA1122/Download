void WebGL2RenderingContextBase::readPixels(
    GLint x,
    GLint y,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    MaybeShared<DOMArrayBufferView> pixels,
    GLuint offset) {
  if (isContextLost())
    return;
  if (bound_pixel_pack_buffer_.Get()) {
    SynthesizeGLError(GL_INVALID_OPERATION, "readPixels",
                      "PIXEL_PACK buffer should not be bound");
    return;
  }

  ReadPixelsHelper(x, y, width, height, format, type, pixels.View(), offset);
}
