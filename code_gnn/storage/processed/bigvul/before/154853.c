error::Error GLES2DecoderPassthroughImpl::DoUniformBlockBinding(
    GLuint program,
    GLuint index,
    GLuint binding) {
  api()->glUniformBlockBindingFn(GetProgramServiceID(program, resources_),
                                 index, binding);
  return error::kNoError;
}
