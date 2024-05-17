bool GLES2DecoderImpl::CheckCurrentProgram(const char* function_name) {
  if (!state_.current_program.get()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name, "no program in use");
    return false;
  }
  if (!state_.current_program->InUse()) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION, function_name, "program not linked");
    return false;
  }
  return true;
}
