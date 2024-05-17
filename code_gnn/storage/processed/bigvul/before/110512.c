void GLES2DecoderImpl::DoUniform1iv(
    GLint fake_location, GLsizei count, const GLint *value) {
  GLenum type = 0;
  GLint real_location = -1;
  if (!PrepForSetUniformByLocation(
      fake_location, "glUniform1iv", &real_location, &type, &count)) {
    return;
  }
  if (type == GL_SAMPLER_2D || type == GL_SAMPLER_2D_RECT_ARB ||
      type == GL_SAMPLER_CUBE || type == GL_SAMPLER_EXTERNAL_OES) {
    if (!current_program_->SetSamplers(
          group_->max_texture_units(), fake_location, count, value)) {
      SetGLError(GL_INVALID_VALUE, "glUniform1iv", "texture unit out of range");
      return;
    }
  }
  glUniform1iv(real_location, count, value);
}
