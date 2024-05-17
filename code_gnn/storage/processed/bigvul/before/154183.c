bool GLES2DecoderImpl::GetServiceTextureId(uint32_t client_texture_id,
                                           uint32_t* service_texture_id) {
  TextureRef* texture_ref = texture_manager()->GetTexture(client_texture_id);
  if (texture_ref) {
    *service_texture_id = texture_ref->service_id();
    return true;
  }
  return false;
}
