error::Error GLES2DecoderPassthroughImpl::DoBindTexture(GLenum target,
                                                        GLuint texture) {
  GLuint service_id =
      GetTextureServiceID(api(), texture, resources_, bind_generates_resource_);

  CheckErrorCallbackState();

  api()->glBindTextureFn(target, service_id);

  if (CheckErrorCallbackState()) {
    return error::kNoError;
  }

  DCHECK(GLenumToTextureTarget(target) != TextureTarget::kUnkown);
  scoped_refptr<TexturePassthrough> texture_passthrough = nullptr;

  RemovePendingBindingTexture(target, active_texture_unit_);

  if (service_id != 0) {
    if (!resources_->texture_object_map.GetServiceID(texture,
                                                     &texture_passthrough)) {
      texture_passthrough = new TexturePassthrough(service_id, target);
      resources_->texture_object_map.SetIDMapping(texture, texture_passthrough);
    } else {
      DCHECK(texture_passthrough->target() == target);
    }

    DCHECK(texture_passthrough);

    if (texture_passthrough->is_bind_pending()) {
      textures_pending_binding_.emplace_back(target, active_texture_unit_,
                                             texture_passthrough->AsWeakPtr());
    }
  }

  BoundTexture* bound_texture =
      &bound_textures_[static_cast<size_t>(GLenumToTextureTarget(target))]
                      [active_texture_unit_];
  bound_texture->client_id = texture;
  bound_texture->texture = std::move(texture_passthrough);

  return error::kNoError;
}
