error::Error GLES2DecoderPassthroughImpl::DoVertexAttribI4i(GLuint indx,
                                                            GLint x,
                                                            GLint y,
                                                            GLint z,
                                                            GLint w) {
  api()->glVertexAttribI4iFn(indx, x, y, z, w);
  return error::kNoError;
}
