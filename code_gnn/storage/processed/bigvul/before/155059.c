void WebGLRenderingContextBase::texImage2D(
    ExecutionContext* execution_context,
    GLenum target,
    GLint level,
    GLint internalformat,
    GLenum format,
    GLenum type,
    CanvasRenderingContextHost* context_host,
    ExceptionState& exception_state) {
  TexImageHelperCanvasRenderingContextHost(
      execution_context->GetSecurityOrigin(), kTexImage2D, target, level,
      internalformat, format, type, 0, 0, 0, context_host,
      GetTextureSourceSize(context_host), 1, 0, exception_state);
}
