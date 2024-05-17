void WebGL2RenderingContextBase::texSubImage2D(
    ExecutionContext* execution_context,
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLsizei width,
    GLsizei height,
    GLenum format,
    GLenum type,
    HTMLImageElement* image,
    ExceptionState& exception_state) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texSubImage2D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }

  TexImageHelperHTMLImageElement(
      execution_context->GetSecurityOrigin(), kTexSubImage2D, target, level, 0,
      format, type, xoffset, yoffset, 0, image,
      GetTextureSourceSubRectangle(width, height), 1, 0, exception_state);
}
