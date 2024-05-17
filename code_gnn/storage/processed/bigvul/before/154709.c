error::Error GLES2DecoderPassthroughImpl::DoGetSynciv(GLuint sync,
                                                      GLenum pname,
                                                      GLsizei bufsize,
                                                      GLsizei* length,
                                                      GLint* values) {
  api()->glGetSyncivFn(GetSyncServiceID(sync, resources_), pname, bufsize,
                       length, values);
  return error::kNoError;
}
