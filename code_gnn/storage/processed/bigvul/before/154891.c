GLuint GetFramebufferServiceID(gl::GLApi* api,
                               GLuint client_id,
                               ClientServiceMap<GLuint, GLuint>* id_map,
                               bool create_if_missing) {
  return GetServiceID(client_id, id_map, create_if_missing, [api]() {
    GLuint service_id = 0;
    api->glGenFramebuffersEXTFn(1, &service_id);
    return service_id;
  });
}
