error::Error GLES2DecoderPassthroughImpl::DoClearBufferfi(GLenum buffer,
                                                          GLint drawbuffers,
                                                          GLfloat depth,
                                                          GLint stencil) {
  api()->glClearBufferfiFn(buffer, drawbuffers, depth, stencil);
  return error::kNoError;
}
