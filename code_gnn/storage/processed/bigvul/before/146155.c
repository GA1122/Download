void WebGL2RenderingContextBase::texSubImage2D(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    ImageBitmap* bitmap,
    ExceptionState& exception_state) {
  DCHECK(bitmap);
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texSubImage2D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  TexImageHelperImageBitmap(kTexSubImage2D, target, level, 0, format, type,
                            xoffset, yoffset, 0, bitmap,
                            GetTextureSourceSubRectangle(width, height), 1, 0,
                            exception_state);
}
