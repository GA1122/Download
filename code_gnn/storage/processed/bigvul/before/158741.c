bool GLES2DecoderImpl::PrepareTexturesForRender() {
  DCHECK(state_.current_program.get());
  bool textures_set = false;
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
        GLenum textarget = GetBindTargetForSamplerType(uniform_info->type);
        const SamplerState& sampler_state = GetSamplerStateForTextureUnit(
            uniform_info->type, texture_unit_index);
        if (!texture_ref ||
            !texture_manager()->CanRenderWithSampler(
                texture_ref, sampler_state)) {
          textures_set = true;
          api()->glActiveTextureFn(GL_TEXTURE0 + texture_unit_index);
          api()->glBindTextureFn(textarget, texture_manager()->black_texture_id(
                                                uniform_info->type));
          if (!texture_ref) {
            LOCAL_RENDER_WARNING(
                std::string("there is no texture bound to the unit ") +
                base::UintToString(texture_unit_index));
          } else {
            LOCAL_RENDER_WARNING(
                std::string("texture bound to texture unit ") +
                base::UintToString(texture_unit_index) +
                " is not renderable. It maybe non-power-of-2 and have"
                " incompatible texture filtering.");
          }
          continue;
        }

        if (textarget != GL_TEXTURE_CUBE_MAP) {
          Texture* texture = texture_ref->texture();
          if (DoBindOrCopyTexImageIfNeeded(texture, textarget,
                                           GL_TEXTURE0 + texture_unit_index)) {
            textures_set = true;
            continue;
          }
        }
      }
    }
  }
  return !textures_set;
}
