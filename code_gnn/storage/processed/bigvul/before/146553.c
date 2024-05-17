void WebGLRenderingContextBase::texSubImage2D(
    ExecutionContext* execution_context,
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLenum format,
    GLenum type,
    HTMLImageElement* image,
    ExceptionState& exception_state) {
  TexImageHelperHTMLImageElement(execution_context->GetSecurityOrigin(),
                                 kTexSubImage2D, target, level, 0, format, type,
                                 xoffset, yoffset, 0, image,
                                 SentinelEmptyRect(), 1, 0, exception_state);
}
