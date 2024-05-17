WebGLShader* WebGLRenderingContextBase::createShader(GLenum type) {
  if (isContextLost())
    return nullptr;
  if (!ValidateShaderType("createShader", type)) {
    return nullptr;
  }

  return WebGLShader::Create(this, type);
}
