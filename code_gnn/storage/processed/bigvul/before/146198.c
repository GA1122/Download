void WebGL2RenderingContextBase::uniform3ui(
    const WebGLUniformLocation* location,
    GLuint v0,
    GLuint v1,
    GLuint v2) {
  if (isContextLost() || !location)
    return;

  if (location->Program() != current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "uniform3ui",
                      "location not for current program");
    return;
  }

  ContextGL()->Uniform3ui(location->Location(), v0, v1, v2);
}
