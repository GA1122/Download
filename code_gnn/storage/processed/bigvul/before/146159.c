void WebGL2RenderingContextBase::texSubImage2D(
    ExecutionContext* execution_context,
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLenum format,
    GLenum type,
    HTMLVideoElement* video,
    ExceptionState& exception_state) {
  WebGLRenderingContextBase::texSubImage2D(execution_context, target, level,
                                           xoffset, yoffset, format, type,
                                           video, exception_state);
}
