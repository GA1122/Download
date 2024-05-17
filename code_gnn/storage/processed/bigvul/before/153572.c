void GLES2Implementation::BindTextureHelper(GLenum target, GLuint texture) {
  bool changed = false;
  TextureUnit& unit = texture_units_[active_texture_unit_];
  switch (target) {
    case GL_TEXTURE_2D:
      if (unit.bound_texture_2d != texture) {
        unit.bound_texture_2d = texture;
        changed = true;
      }
      break;
    case GL_TEXTURE_CUBE_MAP:
      if (unit.bound_texture_cube_map != texture) {
        unit.bound_texture_cube_map = texture;
        changed = true;
      }
      break;
    case GL_TEXTURE_EXTERNAL_OES:
      if (unit.bound_texture_external_oes != texture) {
        unit.bound_texture_external_oes = texture;
        changed = true;
      }
      break;
    case GL_TEXTURE_RECTANGLE_ARB:
      if (unit.bound_texture_rectangle_arb != texture) {
        unit.bound_texture_rectangle_arb = texture;
        changed = true;
      }
      break;
    default:
      changed = true;
      break;
  }
  if (changed) {
    GetIdHandler(SharedIdNamespaces::kTextures)
        ->MarkAsUsedForBind(this, target, texture,
                            &GLES2Implementation::BindTextureStub);
  }
}
