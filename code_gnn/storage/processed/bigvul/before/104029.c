void GLES2DecoderImpl::DoGetShaderiv(
    GLuint shader, GLenum pname, GLint* params) {
  ShaderManager::ShaderInfo* info = GetShaderInfoNotProgram(
      shader, "glGetShaderiv");
  if (!info) {
    return;
  }
  switch (pname) {
    case GL_SHADER_SOURCE_LENGTH:
      *params = info->source() ? info->source()->size() + 1 : 0;
      return;
    case GL_COMPILE_STATUS:
      *params = info->IsValid();
      return;
    case GL_INFO_LOG_LENGTH:
      *params = info->log_info() ? info->log_info()->size() + 1 : 0;
      return;
    default:
      break;
  }
  glGetShaderiv(info->service_id(), pname, params);
}
