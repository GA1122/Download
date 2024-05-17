void WebGLRenderingContextBase::validateProgram(WebGLProgram* program) {
  if (!ValidateWebGLProgramOrShader("validateProgram", program))
    return;
  ContextGL()->ValidateProgram(ObjectOrZero(program));
}
