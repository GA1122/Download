bool GLES2DecoderImpl::PrepareTexturesForRender(bool* textures_set,
                                                const char* function_name) {
  DCHECK(state_.current_program.get());
  *textures_set = false;
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


        Framebuffer* framebuffer =
            GetFramebufferInfoForTarget(GL_DRAW_FRAMEBUFFER);
        if (framebuffer) {
          if (CheckDrawingFeedbackLoopsHelper(
                  framebuffer->GetAttachment(GL_DEPTH_ATTACHMENT), texture_ref,
                  function_name)) {
            return false;
          }

          if (CheckDrawingFeedbackLoopsHelper(
                  framebuffer->GetAttachment(GL_STENCIL_ATTACHMENT),
                  texture_ref, function_name)) {
            return false;
          }
        }

        GLenum textarget = GetBindTargetForSamplerType(uniform_info->type);
        const SamplerState& sampler_state = GetSamplerStateForTextureUnit(
            uniform_info->type, texture_unit_index);
        if (!texture_ref ||
            !texture_manager()->CanRenderWithSampler(
                texture_ref, sampler_state)) {
          *textures_set = true;
          api()->glActiveTextureFn(GL_TEXTURE0 + texture_unit_index);
          api()->glBindTextureFn(textarget, texture_manager()->black_texture_id(
                                                uniform_info->type));
          if (!texture_ref) {
            LOCAL_RENDER_WARNING(
                std::string("there is no texture bound to the unit ") +
                base::NumberToString(texture_unit_index));
          } else {
            LOCAL_RENDER_WARNING(
                std::string("texture bound to texture unit ") +
                base::NumberToString(texture_unit_index) +
                " is not renderable. It maybe non-power-of-2 and have"
                " incompatible texture filtering.");
          }
          continue;
        } else if (!texture_ref->texture()->CompatibleWithSamplerUniformType(
                       uniform_info->type, sampler_state)) {
          LOCAL_SET_GL_ERROR(
              GL_INVALID_OPERATION, function_name,
              (std::string("Texture bound to texture unit ") +
               base::NumberToString(texture_unit_index) +
               " with internal format " +
               GLES2Util::GetStringEnum(
                   texture_ref->texture()->GetInternalFormatOfBaseLevel()) +
               " is not compatible with sampler type " +
               GLES2Util::GetStringEnum(uniform_info->type))
                  .c_str());
          return false;
        }


        if (framebuffer) {
          for (GLsizei kk = 0; kk <= framebuffer->last_color_attachment_id();
               ++kk) {
            GLenum attachment = static_cast<GLenum>(GL_COLOR_ATTACHMENT0 + kk);
            if (CheckDrawingFeedbackLoopsHelper(
                    framebuffer->GetAttachment(attachment), texture_ref,
                    function_name)) {
              return false;
            }
          }
        }

        if (textarget != GL_TEXTURE_CUBE_MAP) {
          Texture* texture = texture_ref->texture();
          if (DoBindOrCopyTexImageIfNeeded(texture, textarget,
                                           GL_TEXTURE0 + texture_unit_index)) {
            *textures_set = true;
            continue;
          }
        }
      }
    }
  }
  return true;
}
