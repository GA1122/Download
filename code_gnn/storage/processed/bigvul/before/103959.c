bool GLES2DecoderImpl::CheckCurrentProgram(const char* function_name) {
  if (!current_program_) {
      SetGLError(GL_INVALID_OPERATION,
                 (std::string(function_name) + ": no program in use").c_str());
      return false;
  }
  if (!current_program_->InUse()) {
    SetGLError(GL_INVALID_OPERATION,
               (std::string(function_name) + ": program not linked").c_str());
    return false;
  }
  return true;
}
