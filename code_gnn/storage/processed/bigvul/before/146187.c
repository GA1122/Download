void WebGL2RenderingContextBase::uniform2ui(
    const WebGLUniformLocation* location,
    GLuint v0,
    GLuint v1) {
  if (isContextLost() || !location)
    return;

  if (location->Program() != current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "uniform2ui",
                      "location not for current program");
    return;
  }

  ContextGL()->Uniform2ui(location->Location(), v0, v1);
}
