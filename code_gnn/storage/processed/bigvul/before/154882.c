error::Error GLES2DecoderPassthroughImpl::DoVertexAttribI4uiv(
    GLuint indx,
    const volatile GLuint* values) {
  api()->glVertexAttribI4uivFn(indx, const_cast<const GLuint*>(values));
  return error::kNoError;
}
