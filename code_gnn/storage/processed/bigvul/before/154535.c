void GLES2DecoderPassthroughImpl::UpdateTextureSizeFromClientID(
    GLuint client_id) {
  scoped_refptr<TexturePassthrough> texture = nullptr;
  if (resources_->texture_object_map.GetServiceID(client_id, &texture)) {
    UpdateTextureSizeFromTexturePassthrough(texture.get(), client_id);
  }
}
