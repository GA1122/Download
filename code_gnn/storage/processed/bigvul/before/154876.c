error::Error GLES2DecoderPassthroughImpl::DoVertexAttrib4f(GLuint indx,
                                                           GLfloat x,
                                                           GLfloat y,
                                                           GLfloat z,
                                                           GLfloat w) {
  api()->glVertexAttrib4fFn(indx, x, y, z, w);
  return error::kNoError;
}
