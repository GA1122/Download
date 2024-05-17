error::Error GLES2DecoderPassthroughImpl::DoShaderSource(GLuint shader,
                                                         GLsizei count,
                                                         const char** string,
                                                         const GLint* length) {
  api()->glShaderSourceFn(GetShaderServiceID(shader, resources_), count, string,
                          length);
  return error::kNoError;
}
