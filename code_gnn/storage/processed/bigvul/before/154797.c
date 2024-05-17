error::Error GLES2DecoderPassthroughImpl::DoShaderBinary(GLsizei n,
                                                         const GLuint* shaders,
                                                         GLenum binaryformat,
                                                         const void* binary,
                                                         GLsizei length) {
  std::vector<GLuint> service_shaders(n, 0);
  for (GLsizei i = 0; i < n; i++) {
    service_shaders[i] = GetShaderServiceID(shaders[i], resources_);
  }
  api()->glShaderBinaryFn(n, service_shaders.data(), binaryformat, binary,
                          length);
  return error::kNoError;
}
