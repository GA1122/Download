error::Error GLES2DecoderPassthroughImpl::DoVertexAttribDivisorANGLE(
    GLuint index,
    GLuint divisor) {
  api()->glVertexAttribDivisorANGLEFn(index, divisor);
  return error::kNoError;
}
