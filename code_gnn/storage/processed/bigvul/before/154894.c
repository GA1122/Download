GLuint GetRenderbufferServiceID(gl::GLApi* api,
                                GLuint client_id,
                                PassthroughResources* resources,
                                bool create_if_missing) {
  return GetServiceID(client_id, &resources->renderbuffer_id_map,
                      create_if_missing, [api]() {
                        GLuint service_id = 0;
                        api->glGenRenderbuffersEXTFn(1, &service_id);
                        return service_id;
                      });
}
