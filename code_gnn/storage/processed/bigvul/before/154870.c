error::Error GLES2DecoderPassthroughImpl::DoVertexAttrib1f(GLuint indx,
                                                           GLfloat x) {
  api()->glVertexAttrib1fFn(indx, x);
  return error::kNoError;
}
