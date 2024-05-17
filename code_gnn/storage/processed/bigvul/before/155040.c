String WebGLRenderingContextBase::getProgramInfoLog(WebGLProgram* program) {
  if (!ValidateWebGLProgramOrShader("getProgramInfoLog", program))
    return String();
  GLStringQuery query(ContextGL());
  return query.Run<GLStringQuery::ProgramInfoLog>(ObjectNonZero(program));
}
