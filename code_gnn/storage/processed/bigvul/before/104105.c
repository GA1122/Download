bool GLES2DecoderImpl::GetServiceTextureId(uint32 client_texture_id,
                                           uint32* service_texture_id) {
  TextureManager::TextureInfo* texture =
      texture_manager()->GetTextureInfo(client_texture_id);
  if (texture) {
    *service_texture_id = texture->service_id();
    return true;
  }
  return false;
}
