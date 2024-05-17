error::Error GLES2DecoderPassthroughImpl::DoVertexAttribI4ui(GLuint indx,
                                                             GLuint x,
                                                             GLuint y,
                                                             GLuint z,
                                                             GLuint w) {
  api()->glVertexAttribI4uiFn(indx, x, y, z, w);
  return error::kNoError;
}
