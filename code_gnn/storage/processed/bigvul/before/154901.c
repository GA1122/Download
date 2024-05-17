GLuint GetVertexArrayServiceID(GLuint client_id,
                               ClientServiceMap<GLuint, GLuint>* id_map) {
  return id_map->GetServiceIDOrInvalid(client_id);
}
