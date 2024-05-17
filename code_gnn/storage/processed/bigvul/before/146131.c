void WebGL2RenderingContextBase::texImage2D(ExecutionContext* execution_context,
                                            GLenum target,
                                            GLint level,
                                            GLint internalformat,
                                            GLsizei width,
                                            GLsizei height,
                                            GLint border,
                                            GLenum format,
                                            GLenum type,
                                            HTMLImageElement* image,
                                            ExceptionState& exception_state) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texImage2D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  TexImageHelperHTMLImageElement(execution_context->GetSecurityOrigin(),
                                 kTexImage2D, target, level, internalformat,
                                 format, type, 0, 0, 0, image,
                                 GetTextureSourceSubRectangle(width, height), 1,
                                 unpack_image_height_, exception_state);
}
