error::Error GLES2DecoderPassthroughImpl::DoVertexAttrib2f(GLuint indx,
                                                           GLfloat x,
                                                           GLfloat y) {
  api()->glVertexAttrib2fFn(indx, x, y);
  return error::kNoError;
}
