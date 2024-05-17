static void RestoreCurrentTextureBindings(ContextState* state, GLenum target) {
  TextureUnit& info = state->texture_units[0];
  GLuint last_id;
  scoped_refptr<TextureRef> texture_ref;
  switch (target) {
    case GL_TEXTURE_2D:
      texture_ref = info.bound_texture_2d;
      break;
    case GL_TEXTURE_CUBE_MAP:
      texture_ref = info.bound_texture_cube_map;
      break;
    case GL_TEXTURE_EXTERNAL_OES:
      texture_ref = info.bound_texture_external_oes;
      break;
    case GL_TEXTURE_RECTANGLE_ARB:
      texture_ref = info.bound_texture_rectangle_arb;
      break;
    default:
      NOTREACHED();
      break;
  }
  if (texture_ref.get()) {
    last_id = texture_ref->service_id();
  } else {
    last_id = 0;
  }

  glBindTexture(target, last_id);
  glActiveTexture(GL_TEXTURE0 + state->active_texture_unit);
}
