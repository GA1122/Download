String WebGLRenderingContextBase::getShaderSource(WebGLShader* shader) {
  if (!ValidateWebGLProgramOrShader("getShaderSource", shader))
    return String();
  return EnsureNotNull(shader->Source());
}
