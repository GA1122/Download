GLuint Get2dServiceId(const TextureUnit& unit) {
  return unit.bound_texture_2d.get()
       ? unit.bound_texture_2d->service_id() : 0;
 }
