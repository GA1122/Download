error::Error GLES2DecoderPassthroughImpl::DoGetAttachedShaders(
    GLuint program,
    GLsizei maxcount,
    GLsizei* count,
    GLuint* shaders) {
  api()->glGetAttachedShadersFn(GetProgramServiceID(program, resources_),
                                maxcount, count, shaders);
  return error::kNoError;
}
