bool GLES2DecoderImpl::DoIsProgram(GLuint client_id) {
  const Program* program = GetProgram(client_id);
  return program != nullptr && !program->IsDeleted();
}
