void WebGLRenderingContextBase::uniform2f(const WebGLUniformLocation* location,
                                          GLfloat x,
                                          GLfloat y) {
  if (isContextLost() || !location)
    return;

  if (location->Program() != current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "uniform2f",
                      "location not for current program");
    return;
  }

  ContextGL()->Uniform2f(location->Location(), x, y);
}
