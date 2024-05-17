void WebGLRenderingContextBase::useProgram(WebGLProgram* program) {
  bool deleted;
  if (!CheckObjectToBeBound("useProgram", program, deleted))
    return;
  if (deleted)
    program = 0;
  if (program && !program->LinkStatus(this)) {
    SynthesizeGLError(GL_INVALID_OPERATION, "useProgram", "program not valid");
    return;
  }

  if (current_program_ != program) {
    if (current_program_)
      current_program_->OnDetached(ContextGL());
    current_program_ = program;
    ContextGL()->UseProgram(ObjectOrZero(program));
    if (program)
      program->OnAttached();
  }
}
