error::Error GLES2DecoderPassthroughImpl::DoGetProgramiv(GLuint program,
                                                         GLenum pname,
                                                         GLsizei bufsize,
                                                         GLsizei* length,
                                                         GLint* params) {
  api()->glGetProgramivRobustANGLEFn(GetProgramServiceID(program, resources_),
                                     pname, bufsize, length, params);
  return error::kNoError;
}
