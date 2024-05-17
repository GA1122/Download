void GLES2DecoderImpl::DoTransformFeedbackVaryings(
    GLuint client_program_id, GLsizei count, const char* const* varyings,
    GLenum buffer_mode) {
  Program* program = GetProgramInfoNotShader(
      client_program_id, "glTransformFeedbackVaryings");
  if (!program) {
    return;
  }
  program->TransformFeedbackVaryings(count, varyings, buffer_mode);
}
