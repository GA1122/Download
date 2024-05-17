void WebGLRenderingContextBase::bindAttribLocation(WebGLProgram* program,
                                                   GLuint index,
                                                   const String& name) {
  if (!ValidateWebGLObject("bindAttribLocation", program))
    return;
  if (!ValidateLocationLength("bindAttribLocation", name))
    return;
  if (IsPrefixReserved(name)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "bindAttribLocation",
                      "reserved prefix");
    return;
  }
  ContextGL()->BindAttribLocation(ObjectOrZero(program), index,
                                  name.Utf8().data());
}
