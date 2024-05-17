GLuint GetOesServiceId(const TextureUnit& unit) {
  return unit.bound_texture_external_oes.get()
      ? unit.bound_texture_external_oes->service_id() : 0;
}
