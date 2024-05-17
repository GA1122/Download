error::Error GLES2DecoderPassthroughImpl::DoVertexAttribI4iv(
    GLuint indx,
    const volatile GLint* values) {
  api()->glVertexAttribI4ivFn(indx, const_cast<const GLint*>(values));
  return error::kNoError;
}
