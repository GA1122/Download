bool GLES2DecoderImpl::ClearUnclearedTextures() {
  if (!texture_manager()->HaveUnsafeTextures()) {
    return true;
  }

  if (current_program_) {
    const ProgramManager::ProgramInfo::SamplerIndices& sampler_indices =
       current_program_->sampler_indices();
    for (size_t ii = 0; ii < sampler_indices.size(); ++ii) {
      const ProgramManager::ProgramInfo::UniformInfo* uniform_info =
          current_program_->GetUniformInfo(sampler_indices[ii]);
      DCHECK(uniform_info);
      for (size_t jj = 0; jj < uniform_info->texture_units.size(); ++jj) {
        GLuint texture_unit_index = uniform_info->texture_units[jj];
        if (texture_unit_index < group_->max_texture_units()) {
          TextureUnit& texture_unit = texture_units_[texture_unit_index];
          TextureManager::TextureInfo* texture_info =
              texture_unit.GetInfoForSamplerType(uniform_info->type);
          if (texture_info && !texture_info->SafeToRenderFrom()) {
            if (!texture_manager()->ClearRenderableLevels(this, texture_info)) {
              return false;
            }
          }
        }
      }
    }
  }
  return true;
}
