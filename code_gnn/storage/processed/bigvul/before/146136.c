void WebGL2RenderingContextBase::texImage2D(ExecutionContext* execution_context,
                                            GLenum target,
                                            GLint level,
                                            GLint internalformat,
                                            GLenum format,
                                            GLenum type,
                                            HTMLCanvasElement* canvas,
                                            ExceptionState& exception_state) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "texImage2D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }

  WebGLRenderingContextBase::texImage2D(execution_context, target, level,
                                        internalformat, format, type, canvas,
                                        exception_state);
}
