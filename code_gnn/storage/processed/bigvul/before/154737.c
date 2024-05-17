error::Error GLES2DecoderPassthroughImpl::DoIsProgram(GLuint program,
                                                      uint32_t* result) {
  *result = api()->glIsProgramFn(GetProgramServiceID(program, resources_));
  return error::kNoError;
}
