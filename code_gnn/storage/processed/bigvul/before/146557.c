void WebGLRenderingContextBase::uniform1f(const WebGLUniformLocation* location,
                                          GLfloat x) {
  if (isContextLost() || !location)
    return;

  if (location->Program() != current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "uniform1f",
                      "location not for current program");
    return;
  }

  ContextGL()->Uniform1f(location->Location(), x);
}
