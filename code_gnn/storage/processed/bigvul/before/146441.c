void WebGLRenderingContextBase::attachShader(WebGLProgram* program,
                                             WebGLShader* shader) {
  if (isContextLost() || !ValidateWebGLObject("attachShader", program) ||
      !ValidateWebGLObject("attachShader", shader))
    return;
  if (!program->AttachShader(shader)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "attachShader",
                      "shader attachment already has shader");
    return;
  }
  ContextGL()->AttachShader(ObjectOrZero(program), ObjectOrZero(shader));
  shader->OnAttached();
}
