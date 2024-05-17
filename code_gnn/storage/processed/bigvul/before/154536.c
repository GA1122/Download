void GLES2DecoderPassthroughImpl::UpdateTextureSizeFromTarget(GLenum target) {
  GLenum texture_type = TextureTargetToTextureType(target);
  TextureTarget internal_texture_type = GLenumToTextureTarget(texture_type);
  DCHECK(internal_texture_type != TextureTarget::kUnkown);
  BoundTexture& bound_texture =
      bound_textures_[static_cast<size_t>(internal_texture_type)]
                     [active_texture_unit_];
  UpdateTextureSizeFromTexturePassthrough(bound_texture.texture.get(),
                                          bound_texture.client_id);
}
