error::Error GLES2DecoderPassthroughImpl::DoGetShaderInfoLog(
    GLuint shader,
    std::string* infolog) {
  CheckErrorCallbackState();

  GLuint service_id = GetShaderServiceID(shader, resources_);
  GLint info_log_len = 0;
  api()->glGetShaderivFn(service_id, GL_INFO_LOG_LENGTH, &info_log_len);
  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  std::vector<char> buffer(info_log_len, 0);
  GLsizei length = 0;
  api()->glGetShaderInfoLogFn(service_id, info_log_len, &length, buffer.data());
  DCHECK(length <= info_log_len);
  *infolog = length > 0 ? std::string(buffer.data(), length) : std::string();
  return error::kNoError;
}
