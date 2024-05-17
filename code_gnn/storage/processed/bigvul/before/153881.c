void GLES2DecoderImpl::BindImage(uint32_t client_texture_id,
                                 uint32_t texture_target,
                                 gl::GLImage* image,
                                 bool can_bind_to_sampler) {
  TextureRef* ref = texture_manager()->GetTexture(client_texture_id);
  if (!ref) {
    return;
  }

  GLenum bind_target = GLES2Util::GLFaceTargetToTextureTarget(texture_target);
  if (ref->texture()->target() != bind_target) {
    return;
  }

  texture_manager()->SetLevelImage(ref, texture_target, 0, image,
                                   can_bind_to_sampler
                                       ? gpu::gles2::Texture::BOUND
                                       : gpu::gles2::Texture::UNBOUND);
}
