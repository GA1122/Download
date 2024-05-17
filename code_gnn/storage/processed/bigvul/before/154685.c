error::Error GLES2DecoderPassthroughImpl::DoGetFragDataLocation(
    GLuint program,
    const char* name,
    GLint* result) {
  *result = api()->glGetFragDataLocationFn(
      GetProgramServiceID(program, resources_), name);
  return error::kNoError;
}
