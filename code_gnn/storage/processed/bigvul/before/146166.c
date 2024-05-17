void WebGL2RenderingContextBase::texSubImage3D(
    ExecutionContext* execution_context,
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLint zoffset,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLenum format,
    GLenum type,
    HTMLVideoElement* video,
    ExceptionState& exception_state) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texSubImage3D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }

  TexImageHelperHTMLVideoElement(execution_context->GetSecurityOrigin(),
                                 kTexSubImage3D, target, level, 0, format, type,
                                 xoffset, yoffset, zoffset, video,
                                 GetTextureSourceSubRectangle(width, height),
                                 depth, unpack_image_height_, exception_state);
}
