void GLES2DecoderImpl::DoUniform1i(GLint fake_location, GLint v0) {
  GLenum type = 0;
  GLsizei count = 1;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(
      fake_location, "glUniform1iv", &real_location, &type, &count)) {
    return;
  }
  if (!current_program_->SetSamplers(
      group_->max_texture_units(), fake_location, 1, &v0)) {
    SetGLError(GL_INVALID_VALUE, "glUniform1i", "texture unit out of range");
    return;
  }
  glUniform1i(real_location, v0);
}