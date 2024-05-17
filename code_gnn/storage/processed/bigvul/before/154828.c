error::Error GLES2DecoderPassthroughImpl::DoTransformFeedbackVaryings(
    GLuint program,
    GLsizei count,
    const char** varyings,
    GLenum buffermode) {
  api()->glTransformFeedbackVaryingsFn(GetProgramServiceID(program, resources_),
                                       count, varyings, buffermode);
  return error::kNoError;
}
