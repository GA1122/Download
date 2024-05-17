String WebGLRenderingContextBase::getShaderInfoLog(WebGLShader* shader) {
  if (isContextLost() || !ValidateWebGLObject("getShaderInfoLog", shader))
    return String();
  GLStringQuery query(ContextGL());
  return query.Run<GLStringQuery::ShaderInfoLog>(ObjectNonZero(shader));
}
