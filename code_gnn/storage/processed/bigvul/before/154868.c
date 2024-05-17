error::Error GLES2DecoderPassthroughImpl::DoUseProgram(GLuint program) {
  api()->glUseProgramFn(GetProgramServiceID(program, resources_));
  return error::kNoError;
}
