void GLES2DecoderImpl::DoUniform1iv(
    GLint location, GLsizei count, const GLint *value) {
  if (!CheckCurrentProgramForUniform(location, "glUniform1iv")) {
    return;
  }
  GLenum type = 0;
  if (!PrepForSetUniformByLocation(location, "glUniform1iv", &type, &count)) {
    return;
  }
  if (type == GL_SAMPLER_2D || type == GL_SAMPLER_CUBE ||
      type == GL_SAMPLER_EXTERNAL_OES) {
    current_program_->SetSamplers(location, count, value);
  }
  glUniform1iv(location, count, value);
}
