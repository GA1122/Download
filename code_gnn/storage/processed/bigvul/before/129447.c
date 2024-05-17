void GLES2DecoderImpl::InitTextureMaxAnisotropyIfNeeded(
    GLenum target, GLenum pname) {
  if (!workarounds().init_texture_max_anisotropy)
    return;
  if (pname != GL_TEXTURE_MAX_ANISOTROPY_EXT ||
      !validators_->texture_parameter.IsValid(pname)) {
    return;
  }

  TextureRef* texture_ref = texture_manager()->GetTextureInfoForTarget(
      &state_, target);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(
        GL_INVALID_OPERATION,
        "glGetTexParamter{fi}v", "unknown texture for target");
    return;
  }
  Texture* texture = texture_ref->texture();
  texture->InitTextureMaxAnisotropyIfNeeded(target);
}
