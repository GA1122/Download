GLuint GetProgramServiceID(GLuint client_id, PassthroughResources* resources) {
  return resources->program_id_map.GetServiceIDOrInvalid(client_id);
}
