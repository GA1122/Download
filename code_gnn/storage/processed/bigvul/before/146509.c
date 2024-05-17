String WebGLRenderingContextBase::getProgramInfoLog(WebGLProgram* program) {
  if (isContextLost() || !ValidateWebGLObject("getProgramInfoLog", program))
    return String();
  GLStringQuery query(ContextGL());
  return query.Run<GLStringQuery::ProgramInfoLog>(ObjectNonZero(program));
}
