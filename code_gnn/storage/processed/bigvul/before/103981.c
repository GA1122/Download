  ShaderManager::ShaderInfo* CreateShaderInfo(
      GLuint client_id,
      GLuint service_id,
      GLenum shader_type) {
    return shader_manager()->CreateShaderInfo(
        client_id, service_id, shader_type);
  }
