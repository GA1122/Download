error::Error GLES2DecoderPassthroughImpl::DoDeleteShader(GLuint shader) {
  return DeleteHelper(
      shader, &resources_->shader_id_map,
      [this](GLuint shader) { api()->glDeleteShaderFn(shader); });
}
