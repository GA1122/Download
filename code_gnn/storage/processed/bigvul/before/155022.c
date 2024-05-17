void WebGLRenderingContextBase::compileShader(WebGLShader* shader) {
  if (!ValidateWebGLProgramOrShader("compileShader", shader))
    return;
  ContextGL()->CompileShader(ObjectOrZero(shader));
}
