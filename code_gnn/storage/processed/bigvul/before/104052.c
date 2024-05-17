void GLES2DecoderImpl::DoUniform1i(GLint location, GLint v0) {
  if (!CheckCurrentProgramForUniform(location, "glUniform1i")) {
    return;
  }
  current_program_->SetSamplers(location, 1, &v0);
  glUniform1i(location, v0);
}
