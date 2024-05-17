GLuint GetSamplerServiceID(GLuint client_id, PassthroughResources* resources) {
  return resources->sampler_id_map.GetServiceIDOrInvalid(client_id);
}
