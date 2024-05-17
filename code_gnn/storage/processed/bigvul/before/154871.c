error::Error GLES2DecoderPassthroughImpl::DoVertexAttrib1fv(
    GLuint indx,
    const volatile GLfloat* values) {
  api()->glVertexAttrib1fvFn(indx, const_cast<const GLfloat*>(values));
  return error::kNoError;
}
