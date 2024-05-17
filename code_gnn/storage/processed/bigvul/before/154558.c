error::Error GLES2DecoderPassthroughImpl::DoBindAttribLocation(
    GLuint program,
    GLuint index,
    const char* name) {
  api()->glBindAttribLocationFn(GetProgramServiceID(program, resources_), index,
                                name);
  return error::kNoError;
}
