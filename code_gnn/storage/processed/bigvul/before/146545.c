void WebGLRenderingContextBase::texImage2D(ExecutionContext* execution_context,
                                           GLenum target,
                                           GLint level,
                                           GLint internalformat,
                                           GLenum format,
                                           GLenum type,
                                           HTMLImageElement* image,
                                           ExceptionState& exception_state) {
  TexImageHelperHTMLImageElement(execution_context->GetSecurityOrigin(),
                                 kTexImage2D, target, level, internalformat,
                                 format, type, 0, 0, 0, image,
                                 SentinelEmptyRect(), 1, 0, exception_state);
}
