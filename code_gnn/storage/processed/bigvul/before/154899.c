GLuint GetTextureServiceID(gl::GLApi* api,
                           GLuint client_id,
                           PassthroughResources* resources,
                           bool create_if_missing) {
  GLuint service_id = resources->texture_id_map.invalid_service_id();
  if (resources->texture_id_map.GetServiceID(client_id, &service_id)) {
    return service_id;
  }

  if (create_if_missing) {
    GLuint service_id = 0;
    api->glGenTexturesFn(1, &service_id);
    resources->texture_id_map.SetIDMapping(client_id, service_id);
    return service_id;
  }

  return resources->texture_id_map.invalid_service_id();
}
