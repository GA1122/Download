error::Error GLES2DecoderPassthroughImpl::DoValidateProgram(GLuint program) {
  api()->glValidateProgramFn(GetProgramServiceID(program, resources_));
  return error::kNoError;
}
