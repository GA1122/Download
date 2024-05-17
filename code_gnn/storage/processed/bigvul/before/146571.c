void WebGLRenderingContextBase::uniform3i(const WebGLUniformLocation* location,
                                          GLint x,
                                          GLint y,
                                          GLint z) {
  if (isContextLost() || !location)
    return;

  if (location->Program() != current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "uniform3i",
                      "location not for current program");
    return;
  }

  ContextGL()->Uniform3i(location->Location(), x, y, z);
}
