GLuint GetArbServiceId(const TextureUnit& unit) {
  return unit.bound_texture_rectangle_arb.get()
      ? unit.bound_texture_rectangle_arb->service_id() : 0;
}
