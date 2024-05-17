TextureBase* GLES2DecoderPassthroughImpl::GetTextureBase(uint32_t client_id) {
  scoped_refptr<TexturePassthrough> texture = nullptr;
  if (resources_->texture_object_map.GetServiceID(client_id, &texture)) {
    return texture.get();
  } else {
    return nullptr;
  }
}
