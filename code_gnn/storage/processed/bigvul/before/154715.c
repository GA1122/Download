error::Error GLES2DecoderPassthroughImpl::DoGetUniformBlockIndex(
    GLuint program,
    const char* name,
    GLint* index) {
  *index = api()->glGetUniformBlockIndexFn(
      GetProgramServiceID(program, resources_), name);
  return error::kNoError;
}
