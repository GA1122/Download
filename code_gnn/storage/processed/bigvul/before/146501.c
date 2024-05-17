GLint WebGLRenderingContextBase::getAttribLocation(WebGLProgram* program,
                                                   const String& name) {
  if (isContextLost() || !ValidateWebGLObject("getAttribLocation", program))
    return -1;
  if (!ValidateLocationLength("getAttribLocation", name))
    return -1;
  if (!ValidateString("getAttribLocation", name))
    return -1;
  if (IsPrefixReserved(name))
    return -1;
  if (!program->LinkStatus(this)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "getAttribLocation",
                      "program not linked");
    return 0;
  }
  return ContextGL()->GetAttribLocation(ObjectOrZero(program),
                                        name.Utf8().data());
}
