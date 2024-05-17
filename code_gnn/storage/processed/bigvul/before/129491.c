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
            texture_unit.GetInfoForSamplerType(uniform_info->type).get();
        if (!texture_ref || !texture_manager()->CanRender(texture_ref)) {
          glActiveTexture(GL_TEXTURE0 + texture_unit_index);
          texture_ref = texture_unit.bind_target == GL_TEXTURE_2D
                            ? texture_unit.bound_texture_2d.get()
                            : texture_unit.bound_texture_cube_map.get();
          glBindTexture(texture_unit.bind_target,
                        texture_ref ? texture_ref->service_id() : 0);
          continue;
        }

        if (texture_unit.bind_target != GL_TEXTURE_CUBE_MAP) {
          Texture* texture = texture_ref->texture();
          gfx::GLImage* image =
              texture->GetLevelImage(texture_unit.bind_target, 0);
          if (image && !texture->IsAttachedToFramebuffer()) {
            ScopedGLErrorSuppressor suppressor(
                "GLES2DecoderImpl::RestoreStateForTextures", GetErrorState());
            glActiveTexture(GL_TEXTURE0 + texture_unit_index);
            image->DidUseTexImage();
            continue;
          }
        }
      }
    }
  }
  glActiveTexture(GL_TEXTURE0 + state_.active_texture_unit);
}
