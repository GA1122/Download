  Shader* CreateShader(
      GLuint client_id,
      GLuint service_id,
      GLenum shader_type) {
    return shader_manager()->CreateShader(
        client_id, service_id, shader_type);
  }
