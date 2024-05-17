error::Error GLES2DecoderPassthroughImpl::DoBindFragDataLocationEXT(
    GLuint program,
    GLuint colorNumber,
    const char* name) {
  api()->glBindFragDataLocationFn(GetProgramServiceID(program, resources_),
                                  colorNumber, name);
  return error::kNoError;
}
