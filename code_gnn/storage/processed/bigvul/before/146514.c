String WebGLRenderingContextBase::getShaderSource(WebGLShader* shader) {
  if (isContextLost() || !ValidateWebGLObject("getShaderSource", shader))
    return String();
  return EnsureNotNull(shader->Source());
}
