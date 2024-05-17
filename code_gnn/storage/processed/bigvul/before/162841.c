void GLES2DecoderRestoreStateTest::InitializeContextState(
    ContextState* state,
    uint32_t non_default_unit,
    uint32_t active_unit) {
  state->texture_units.resize(group().max_texture_units());
  for (uint32_t tt = 0; tt < state->texture_units.size(); ++tt) {
    TextureRef* ref_cube_map =
        group().texture_manager()->GetDefaultTextureInfo(GL_TEXTURE_CUBE_MAP);
    state->texture_units[tt].bound_texture_cube_map = ref_cube_map;
    TextureRef* ref_2d =
        (tt == non_default_unit)
            ? group().texture_manager()->GetTexture(client_texture_id_)
            : group().texture_manager()->GetDefaultTextureInfo(GL_TEXTURE_2D);
    state->texture_units[tt].bound_texture_2d = ref_2d;
  }
  state->active_texture_unit = active_unit;

  state->sampler_units.resize(group().max_texture_units());
}
