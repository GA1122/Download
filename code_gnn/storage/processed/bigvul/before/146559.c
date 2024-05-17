void WebGLRenderingContextBase::uniform1i(const WebGLUniformLocation* location,
                                          GLint x) {
  if (isContextLost() || !location)
    return;

  if (location->Program() != current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "uniform1i",
                      "location not for current program");
    return;
  }

  ContextGL()->Uniform1i(location->Location(), x);
}
