void WebGLRenderingContextBase::uniform4i(const WebGLUniformLocation* location,
                                          GLint x,
                                          GLint y,
                                          GLint z,
                                          GLint w) {
  if (isContextLost() || !location)
    return;

  if (location->Program() != current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "uniform4i",
                      "location not for current program");
    return;
  }

  ContextGL()->Uniform4i(location->Location(), x, y, z, w);
}
