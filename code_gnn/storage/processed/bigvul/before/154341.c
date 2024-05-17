void GLES2DecoderImpl::RestoreAllExternalTextureBindingsIfNeeded() {
  if (texture_manager()->GetServiceIdGeneration() ==
      texture_manager_service_id_generation_)
    return;

  for (unsigned texture_unit_index = 0;
       texture_unit_index < state_.texture_units.size(); texture_unit_index++) {
    TextureUnit& texture_unit = state_.texture_units[texture_unit_index];
    if (texture_unit.bind_target != GL_TEXTURE_EXTERNAL_OES)
      continue;

    if (TextureRef* texture_ref =
            texture_unit.bound_texture_external_oes.get()) {
      api()->glActiveTextureFn(GL_TEXTURE0 + texture_unit_index);
      api()->glBindTextureFn(GL_TEXTURE_EXTERNAL_OES,
                             texture_ref->service_id());
    }
  }

  api()->glActiveTextureFn(GL_TEXTURE0 + state_.active_texture_unit);

  texture_manager_service_id_generation_ =
      texture_manager()->GetServiceIdGeneration();
}
