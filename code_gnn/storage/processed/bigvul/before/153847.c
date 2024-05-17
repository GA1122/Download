void GLES2Implementation::UseProgramHelper(GLuint program) {
  if (current_program_ != program) {
    current_program_ = program;
    helper_->UseProgram(program);
  }
}
