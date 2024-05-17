bool GLES2DecoderImpl::CheckCurrentProgram(const char* function_name) {
  if (!current_program_) {
    SetGLError(GL_INVALID_OPERATION, function_name, "no program in use");
    return false;
  }
  if (!current_program_->InUse()) {
    SetGLError(GL_INVALID_OPERATION, function_name, "program not linked");
    return false;
  }
  return true;
}
