void WebGLRenderingContextBase::texImage2D(ExecutionContext* execution_context,
                                           GLenum target,
                                           GLint level,
                                           GLint internalformat,
                                           GLenum format,
                                           GLenum type,
                                           HTMLCanvasElement* canvas,
                                           ExceptionState& exception_state) {
  TexImageHelperHTMLCanvasElement(
      execution_context->GetSecurityOrigin(), kTexImage2D, target, level,
      internalformat, format, type, 0, 0, 0, canvas,
      GetTextureSourceSize(canvas), 1, 0, exception_state);
}
