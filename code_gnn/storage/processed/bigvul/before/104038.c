void GLES2DecoderImpl::DoLinkProgram(GLuint program) {
  TRACE_EVENT0("gpu", "GLES2DecoderImpl::DoLinkProgram");
  ProgramManager::ProgramInfo* info = GetProgramInfoNotShader(
      program, "glLinkProgram");
  if (!info) {
    return;
  }

  info->Link();
};
