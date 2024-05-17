void WebGLRenderingContextBase::uniform2i(const WebGLUniformLocation* location,
                                          GLint x,
                                          GLint y) {
  if (isContextLost() || !location)
    return;

  if (location->Program() != current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "uniform2i",
                      "location not for current program");
    return;
  }

  ContextGL()->Uniform2i(location->Location(), x, y);
}
