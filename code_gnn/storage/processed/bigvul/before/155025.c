void WebGLRenderingContextBase::detachShader(WebGLProgram* program,
                                             WebGLShader* shader) {
  if (!ValidateWebGLProgramOrShader("detachShader", program) ||
      !ValidateWebGLProgramOrShader("detachShader", shader))
    return;
  if (!program->DetachShader(shader)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "detachShader",
                      "shader not attached");
    return;
  }
  ContextGL()->DetachShader(ObjectOrZero(program), ObjectOrZero(shader));
  shader->OnDetached(ContextGL());
}
