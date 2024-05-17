void WebGL2RenderingContextBase::uniform1ui(
    const WebGLUniformLocation* location,
    GLuint v0) {
  if (isContextLost() || !location)
    return;

  if (location->Program() != current_program_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "uniform1ui",
                      "location not for current program");
    return;
  }

  ContextGL()->Uniform1ui(location->Location(), v0);
}
