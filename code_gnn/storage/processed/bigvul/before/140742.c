void GLES2DecoderImpl::DoGetShaderiv(
    GLuint shader_id, GLenum pname, GLint* params) {
  Shader* shader = GetShaderInfoNotProgram(shader_id, "glGetShaderiv");
  if (!shader) {
    return;
  }

  switch (pname) {
    case GL_COMPILE_STATUS:
    case GL_INFO_LOG_LENGTH:
    case GL_TRANSLATED_SHADER_SOURCE_LENGTH_ANGLE:
      shader->DoCompile();
      break;

    default:
    break;
  }

  switch (pname) {
    case GL_SHADER_SOURCE_LENGTH:
      *params = shader->source().size();
      if (*params)
        ++(*params);
      return;
    case GL_COMPILE_STATUS:
      *params = compile_shader_always_succeeds_ ? true : shader->valid();
      return;
    case GL_INFO_LOG_LENGTH:
      *params = shader->log_info().size();
      if (*params)
        ++(*params);
      return;
    case GL_TRANSLATED_SHADER_SOURCE_LENGTH_ANGLE:
      *params = shader->translated_source().size();
      if (*params)
        ++(*params);
      return;
    default:
      break;
  }
  glGetShaderiv(shader->service_id(), pname, params);
}
