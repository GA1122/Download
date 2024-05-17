error::Error GLES2DecoderPassthroughImpl::DoEnableVertexAttribArray(
    GLuint index) {
  api()->glEnableVertexAttribArrayFn(index);
  return error::kNoError;
}
