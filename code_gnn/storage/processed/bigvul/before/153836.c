void GLES2Implementation::UnbindTexturesHelper(GLsizei n,
                                               const GLuint* textures) {
  for (GLsizei ii = 0; ii < n; ++ii) {
    for (GLint tt = 0; tt < capabilities_.max_combined_texture_image_units;
         ++tt) {
      TextureUnit& unit = texture_units_[tt];
      if (textures[ii] == unit.bound_texture_2d) {
        unit.bound_texture_2d = 0;
      }
      if (textures[ii] == unit.bound_texture_cube_map) {
        unit.bound_texture_cube_map = 0;
      }
      if (textures[ii] == unit.bound_texture_external_oes) {
        unit.bound_texture_external_oes = 0;
      }
      if (textures[ii] == unit.bound_texture_rectangle_arb) {
        unit.bound_texture_rectangle_arb = 0;
      }
    }
  }
}
