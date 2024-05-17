error::Error GLES2DecoderPassthroughImpl::DoGetInteger64i_v(GLenum pname,
                                                            GLuint index,
                                                            GLsizei bufsize,
                                                            GLsizei* length,
                                                            GLint64* data) {
  glGetInteger64i_vRobustANGLE(pname, index, bufsize, length, data);
  return error::kNoError;
}
