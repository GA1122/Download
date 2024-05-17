WebGLShader* WebGLRenderingContextBase::createShader(GLenum type) {
  if (isContextLost())
    return nullptr;
  if (type != GL_VERTEX_SHADER && type != GL_FRAGMENT_SHADER) {
    SynthesizeGLError(GL_INVALID_ENUM, "createShader", "invalid shader type");
    return nullptr;
  }

  return WebGLShader::Create(this, type);
}
