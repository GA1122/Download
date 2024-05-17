bool GLES2DecoderImpl::PrepareTexturesForRender() {
  DCHECK(state_.current_program.get());
  if (!texture_manager()->HaveUnrenderableTextures() &&
      !texture_manager()->HaveImages()) {
    return true;
  }

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
            texture_unit.GetInfoForSamplerType(uniform_info->type).get();
        GLenum textarget = GetBindTargetForSamplerType(uniform_info->type);
        if (!texture_ref || !texture_manager()->CanRender(texture_ref)) {
          textures_set = true;
          glActiveTexture(GL_TEXTURE0 + texture_unit_index);
          glBindTexture(
              textarget,
              texture_manager()->black_texture_id(uniform_info->type));
          if (!texture_ref) {
            LOCAL_RENDER_WARNING(
                std::string("there is no texture bound to the unit ") +
                base::IntToString(texture_unit_index));
          } else {
            LOCAL_RENDER_WARNING(
                std::string("texture bound to texture unit ") +
                base::IntToString(texture_unit_index) +
                " is not renderable. It maybe non-power-of-2 and have"
                " incompatible texture filtering.");
          }
          continue;
        }

        if (textarget != GL_TEXTURE_CUBE_MAP) {
          Texture* texture = texture_ref->texture();
          gfx::GLImage* image = texture->GetLevelImage(textarget, 0);
          if (image && !texture->IsAttachedToFramebuffer()) {
            ScopedGLErrorSuppressor suppressor(
                "GLES2DecoderImpl::PrepareTexturesForRender", GetErrorState());
            textures_set = true;
            glActiveTexture(GL_TEXTURE0 + texture_unit_index);
            image->WillUseTexImage();
            continue;
          }
        }
      }
    }
  }
  return !textures_set;
}
