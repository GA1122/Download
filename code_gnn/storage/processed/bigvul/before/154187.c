TextureBase* GLES2DecoderImpl::GetTextureBase(uint32_t client_id) {
  TextureRef* texture_ref = texture_manager()->GetTexture(client_id);
  return texture_ref ? texture_ref->texture() : nullptr;
}
