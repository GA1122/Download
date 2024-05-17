bool GLES2DecoderImpl::CheckDrawingFeedbackLoops() {
  Framebuffer* framebuffer = GetFramebufferInfoForTarget(GL_FRAMEBUFFER);
  if (!framebuffer)
    return false;
  const Framebuffer::Attachment* attachment =
      framebuffer->GetAttachment(GL_COLOR_ATTACHMENT0);
  if (!attachment)
    return false;

  DCHECK(state_.current_program.get());
  const Program::SamplerIndices& sampler_indices =
      state_.current_program->sampler_indices();
  for (size_t ii = 0; ii < sampler_indices.size(); ++ii) {
    const Program::UniformInfo* uniform_info =
        state_.current_program->GetUniformInfo(sampler_indices[ii]);
    DCHECK(uniform_info);
    if (uniform_info->type != GL_SAMPLER_2D)
      continue;
    for (size_t jj = 0; jj < uniform_info->texture_units.size(); ++jj) {
      GLuint texture_unit_index = uniform_info->texture_units[jj];
      if (texture_unit_index >= state_.texture_units.size())
        continue;
      TextureUnit& texture_unit = state_.texture_units[texture_unit_index];
      TextureRef* texture_ref =
          texture_unit.GetInfoForSamplerType(GL_SAMPLER_2D).get();
      if (attachment->IsTexture(texture_ref))
        return true;
    }
  }
  return false;
}
