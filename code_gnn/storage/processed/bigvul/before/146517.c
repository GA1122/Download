WebGLUniformLocation* WebGLRenderingContextBase::getUniformLocation(
    WebGLProgram* program,
    const String& name) {
  if (isContextLost() || !ValidateWebGLObject("getUniformLocation", program))
    return nullptr;
  if (!ValidateLocationLength("getUniformLocation", name))
    return nullptr;
  if (!ValidateString("getUniformLocation", name))
    return nullptr;
  if (IsPrefixReserved(name))
    return nullptr;
  if (!program->LinkStatus(this)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "getUniformLocation",
                      "program not linked");
    return nullptr;
  }
  GLint uniform_location = ContextGL()->GetUniformLocation(
      ObjectOrZero(program), name.Utf8().data());
  if (uniform_location == -1)
    return nullptr;
  return WebGLUniformLocation::Create(program, uniform_location);
}
