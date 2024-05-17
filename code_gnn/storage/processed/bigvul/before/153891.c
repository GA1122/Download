bool GLES2DecoderImpl::CheckCurrentProgramForUniform(
    GLint location, const char* function_name) {
  if (!CheckCurrentProgram(function_name)) {
    return false;
  }
  return !state_.current_program->IsInactiveUniformLocationByFakeLocation(
      location);
}
