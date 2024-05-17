void GLES2DecoderImpl::DoGetProgramiv(GLuint program_id,
                                      GLenum pname,
                                      GLint* params,
                                      GLsizei params_size) {
  Program* program = GetProgramInfoNotShader(program_id, "glGetProgramiv");
  if (!program) {
    return;
  }
  program->GetProgramiv(pname, params);
}
