error::Error GLES2DecoderPassthroughImpl::DoGetUniformIndices(
    GLuint program,
    GLsizei count,
    const char* const* names,
    GLsizei bufSize,
    GLuint* indices) {
  api()->glGetUniformIndicesFn(GetProgramServiceID(program, resources_), count,
                               names, indices);
  return error::kNoError;
}
