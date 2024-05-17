error::Error GLES2DecoderPassthroughImpl::DoGetUniformuiv(GLuint program,
                                                          GLint location,
                                                          GLsizei bufsize,
                                                          GLsizei* length,
                                                          GLuint* params) {
  api()->glGetUniformuivRobustANGLEFn(GetProgramServiceID(program, resources_),
                                      location, bufsize * sizeof(*params),
                                      length, params);
  return error::kNoError;
}
