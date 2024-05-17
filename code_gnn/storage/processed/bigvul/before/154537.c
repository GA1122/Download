void GLES2DecoderPassthroughImpl::UpdateTextureSizeFromTexturePassthrough(
    TexturePassthrough* texture,
    GLuint client_id) {
  if (texture == nullptr) {
    return;
  }

  CheckErrorCallbackState();

  GLenum target = texture->target();
  TextureTarget internal_texture_type = GLenumToTextureTarget(target);
  BoundTexture& bound_texture =
      bound_textures_[static_cast<size_t>(internal_texture_type)]
                     [active_texture_unit_];
  bool needs_rebind = bound_texture.texture == texture;
  if (needs_rebind) {
    glBindTexture(target, texture->service_id());
  }

  UpdateBoundTexturePassthroughSize(api(), texture);

  if (client_id != 0) {
    group_->passthrough_discardable_manager()->UpdateTextureSize(
        client_id, group_.get(), texture->estimated_size());
  }

  if (needs_rebind) {
    GLuint old_texture =
        bound_texture.texture ? bound_texture.texture->service_id() : 0;
    glBindTexture(target, old_texture);
  }

  DCHECK(!CheckErrorCallbackState());
}
