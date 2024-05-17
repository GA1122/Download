void WebGL2RenderingContextBase::texImage3D(ExecutionContext* execution_context,
                                            GLenum target,
                                            GLint level,
                                            GLint internalformat,
                                            GLsizei width,
                                            GLsizei height,
                                            GLsizei depth,
                                            GLint border,
                                            GLenum format,
                                            GLenum type,
                                            HTMLImageElement* image,
                                            ExceptionState& exception_state) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texImage3D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }

  TexImageHelperHTMLImageElement(execution_context->GetSecurityOrigin(),
                                 kTexImage3D, target, level, internalformat,
                                 format, type, 0, 0, 0, image,
                                 GetTextureSourceSubRectangle(width, height),
                                 depth, unpack_image_height_, exception_state);
}
