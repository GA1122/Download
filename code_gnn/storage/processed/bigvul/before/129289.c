void GLES2DecoderImpl::DoGetShaderiv(
    GLuint shader_id, GLenum pname, GLint* params) {
  Shader* shader = GetShaderInfoNotProgram(shader_id, "glGetShaderiv");
  if (!shader) {
    return;
  }
  switch (pname) {
    case GL_SHADER_SOURCE_LENGTH:
      *params = shader->source() ? shader->source()->size() + 1 : 0;
      return;
    case GL_COMPILE_STATUS:
      *params = compile_shader_always_succeeds_ ? true : shader->IsValid();
      return;
    case GL_INFO_LOG_LENGTH:
      *params = shader->log_info() ? shader->log_info()->size() + 1 : 0;
      return;
    case GL_TRANSLATED_SHADER_SOURCE_LENGTH_ANGLE:
      *params = shader->translated_source() ?
          shader->translated_source()->size() + 1 : 0;
      return;
    default:
      break;
  }
  glGetShaderiv(shader->service_id(), pname, params);
}
