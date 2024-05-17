GLuint GetShaderServiceID(GLuint client_id, PassthroughResources* resources) {
  return resources->shader_id_map.GetServiceIDOrInvalid(client_id);
}
