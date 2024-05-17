error::Error GLES2DecoderPassthroughImpl::DoVertexAttrib2fv(
    GLuint indx,
    const volatile GLfloat* values) {
  api()->glVertexAttrib2fvFn(indx, const_cast<const GLfloat*>(values));
  return error::kNoError;
}
