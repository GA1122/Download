void PassthroughResources::Destroy(gl::GLApi* api) {
  bool have_context = !!api;
  DeleteServiceObjects(&texture_id_map, have_context,
                       [this, api](GLuint client_id, GLuint texture) {
                         if (!texture_object_map.HasClientID(client_id)) {
                           api->glDeleteTexturesFn(1, &texture);
                         }
                       });
  DeleteServiceObjects(&buffer_id_map, have_context,
                       [api](GLuint client_id, GLuint buffer) {
                         api->glDeleteBuffersARBFn(1, &buffer);
                       });
  DeleteServiceObjects(&renderbuffer_id_map, have_context,
                       [api](GLuint client_id, GLuint renderbuffer) {
                         api->glDeleteRenderbuffersEXTFn(1, &renderbuffer);
                       });
  DeleteServiceObjects(&sampler_id_map, have_context,
                       [api](GLuint client_id, GLuint sampler) {
                         api->glDeleteSamplersFn(1, &sampler);
                       });
  DeleteServiceObjects(&program_id_map, have_context,
                       [api](GLuint client_id, GLuint program) {
                         api->glDeleteProgramFn(program);
                       });
  DeleteServiceObjects(&shader_id_map, have_context,
                       [api](GLuint client_id, GLuint shader) {
                         api->glDeleteShaderFn(shader);
                       });
  DeleteServiceObjects(&sync_id_map, have_context,
                       [api](GLuint client_id, uintptr_t sync) {
                         api->glDeleteSyncFn(reinterpret_cast<GLsync>(sync));
                       });

  if (!have_context) {
    texture_object_map.ForEach(
        [](GLuint client_id, scoped_refptr<TexturePassthrough> texture) {
          texture->MarkContextLost();
        });
    for (const auto& pair : texture_shared_image_map) {
      pair.second->OnContextLost();
    }
  }
  texture_object_map.Clear();
  texture_shared_image_map.clear();
  DestroyPendingTextures(have_context);
}
