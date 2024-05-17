error::Error GLES2DecoderPassthroughImpl::DoBindUniformLocationCHROMIUM(
    GLuint program,
    GLint location,
    const char* name) {
  api()->glBindUniformLocationCHROMIUMFn(
      GetProgramServiceID(program, resources_), location, name);
  return error::kNoError;
}
