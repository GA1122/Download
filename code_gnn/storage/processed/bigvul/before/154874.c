error::Error GLES2DecoderPassthroughImpl::DoVertexAttrib3f(GLuint indx,
                                                           GLfloat x,
                                                           GLfloat y,
                                                           GLfloat z) {
  api()->glVertexAttrib3fFn(indx, x, y, z);
  return error::kNoError;
}
