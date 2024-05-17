    void Unbind(TextureManager::TextureInfo* texture) {
      if (bound_texture_2d == texture) {
        bound_texture_2d = NULL;
      }
      if (bound_texture_cube_map == texture) {
        bound_texture_cube_map = NULL;
      }
      if (bound_texture_external_oes == texture) {
        bound_texture_external_oes = NULL;
      }
    }
