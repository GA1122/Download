GLES2DecoderPassthroughImpl::DoInitializeDiscardableTextureCHROMIUM(
    GLuint texture_id,
    ServiceDiscardableHandle&& discardable_handle) {
  scoped_refptr<TexturePassthrough> texture_passthrough = nullptr;
  if (!resources_->texture_object_map.GetServiceID(texture_id,
                                                   &texture_passthrough) ||
      texture_passthrough == nullptr) {
    InsertError(GL_INVALID_VALUE, "Invalid texture ID");
    return error::kNoError;
  }

  group_->passthrough_discardable_manager()->InitializeTexture(
      texture_id, group_.get(), texture_passthrough->estimated_size(),
      std::move(discardable_handle));

  return error::kNoError;
}
