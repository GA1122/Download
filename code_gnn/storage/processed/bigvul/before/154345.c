static void RestoreCurrentTextureBindings(ContextState* state,
                                          GLenum target,
                                          GLuint texture_unit) {
  DCHECK(!state->texture_units.empty());
  DCHECK_LT(texture_unit, state->texture_units.size());
  TextureUnit& info = state->texture_units[texture_unit];
  GLuint last_id;
  TextureRef* texture_ref = info.GetInfoForTarget(target);
  if (texture_ref) {
    last_id = texture_ref->service_id();
  } else {
    last_id = 0;
  }

  state->api()->glBindTextureFn(target, last_id);
}
