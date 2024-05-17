error::Error GLES2DecoderPassthroughImpl::DoVertexAttrib4fv(
    GLuint indx,
    const volatile GLfloat* values) {
  api()->glVertexAttrib4fvFn(indx, const_cast<const GLfloat*>(values));
  return error::kNoError;
}
