GLint WebGL2RenderingContextBase::getFragDataLocation(WebGLProgram* program,
                                                      const String& name) {
  if (isContextLost() || !ValidateWebGLObject("getFragDataLocation", program))
    return -1;

  return ContextGL()->GetFragDataLocation(ObjectOrZero(program),
                                          name.Utf8().data());
}
