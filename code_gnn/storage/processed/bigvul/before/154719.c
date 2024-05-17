error::Error GLES2DecoderPassthroughImpl::DoGetUniformfv(GLuint program,
                                                         GLint location,
                                                         GLsizei bufsize,
                                                         GLsizei* length,
                                                         GLfloat* params) {
  api()->glGetUniformfvRobustANGLEFn(GetProgramServiceID(program, resources_),
                                     location, bufsize * sizeof(*params),
                                     length, params);
  return error::kNoError;
}
