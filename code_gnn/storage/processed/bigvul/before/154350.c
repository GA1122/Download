void GLES2DecoderImpl::RestoreStateForTextures() {
  DCHECK(state_.current_program.get());
  const Program::SamplerIndices& sampler_indices =
      state_.current_program->sampler_indices();
  for (size_t ii = 0; ii < sampler_indices.size(); ++ii) {
    const Program::UniformInfo* uniform_info =
        state_.current_program->GetUniformInfo(sampler_indices[ii]);
    DCHECK(uniform_info);
    for (size_t jj = 0; jj < uniform_info->texture_units.size(); ++jj) {
      GLuint texture_unit_index = uniform_info->texture_units[jj];
      if (texture_unit_index < state_.texture_units.size()) {
        TextureUnit& texture_unit = state_.texture_units[texture_unit_index];
        TextureRef* texture_ref =
            texture_unit.GetInfoForSamplerType(uniform_info->type);
        const SamplerState& sampler_state = GetSamplerStateForTextureUnit(
            uniform_info->type, texture_unit_index);
        if (!texture_ref ||
            !texture_manager()->CanRenderWithSampler(
                texture_ref, sampler_state)) {
          api()->glActiveTextureFn(GL_TEXTURE0 + texture_unit_index);
          texture_ref =
              texture_unit.GetInfoForTarget(texture_unit.bind_target);
          api()->glBindTextureFn(texture_unit.bind_target,
                                 texture_ref ? texture_ref->service_id() : 0);
          continue;
        }
      }
    }
  }
  api()->glActiveTextureFn(GL_TEXTURE0 + state_.active_texture_unit);
}
