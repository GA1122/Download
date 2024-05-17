String WebGLRenderingContextBase::getShaderInfoLog(WebGLShader* shader) {
  if (!ValidateWebGLProgramOrShader("getShaderInfoLog", shader))
    return String();
  GLStringQuery query(ContextGL());
  return query.Run<GLStringQuery::ShaderInfoLog>(ObjectNonZero(shader));
}
