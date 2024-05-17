error::Error GLES2DecoderPassthroughImpl::DoGetShaderSource(
    GLuint shader,
    std::string* source) {
  CheckErrorCallbackState();

  GLuint shader_service_id = GetShaderServiceID(shader, resources_);
  GLint shader_source_length = 0;
  api()->glGetShaderivFn(shader_service_id, GL_SHADER_SOURCE_LENGTH,
                         &shader_source_length);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  std::vector<char> buffer(shader_source_length, 0);
  GLsizei length = 0;
  api()->glGetShaderSourceFn(shader_service_id, shader_source_length, &length,
                             buffer.data());
  DCHECK(length <= shader_source_length);
  *source = shader_source_length > 0 ? std::string(buffer.data(), length)
                                     : std::string();
  return error::kNoError;
}
