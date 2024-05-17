void WebGLRenderingContextBase::compileShader(WebGLShader* shader) {
  if (isContextLost() || !ValidateWebGLObject("compileShader", shader))
    return;
  ContextGL()->CompileShader(ObjectOrZero(shader));
}
