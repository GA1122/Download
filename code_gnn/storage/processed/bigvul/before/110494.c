bool GLES2DecoderImpl::DoIsTexture(GLuint client_id) {
  const TextureManager::TextureInfo* texture = GetTextureInfo(client_id);
  return texture && texture->IsValid() && !texture->IsDeleted();
}
