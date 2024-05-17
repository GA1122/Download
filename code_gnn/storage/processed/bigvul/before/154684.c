error::Error GLES2DecoderPassthroughImpl::DoGetFragDataIndexEXT(
    GLuint program,
    const char* name,
    GLint* index) {
  *index = api()->glGetFragDataIndexFn(GetProgramServiceID(program, resources_),
                                       name);
  return error::kNoError;
}
