void GLES2DecoderPassthroughImpl::BindImage(uint32_t client_texture_id,
                                            uint32_t texture_target,
                                            gl::GLImage* image,
                                            bool can_bind_to_sampler) {
  scoped_refptr<TexturePassthrough> passthrough_texture = nullptr;
  if (!resources_->texture_object_map.GetServiceID(client_texture_id,
                                                   &passthrough_texture)) {
    return;
  }

  DCHECK(passthrough_texture != nullptr);

  passthrough_texture->set_is_bind_pending(!can_bind_to_sampler);

  GLenum bind_target = GLES2Util::GLFaceTargetToTextureTarget(texture_target);
  if (passthrough_texture->target() != bind_target) {
    return;
  }

  passthrough_texture->SetLevelImage(texture_target, 0, image);
}
