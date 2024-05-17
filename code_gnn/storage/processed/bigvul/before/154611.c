GLES2DecoderPassthroughImpl::DoCreateAndTexStorage2DSharedImageINTERNAL(
    GLuint texture_client_id,
    GLenum internalformat,
    const volatile GLbyte* mailbox) {
  if (internalformat != GL_NONE) {
    InsertError(GL_INVALID_ENUM, "internal format not supported.");
    return error::kNoError;
  }

  if (!texture_client_id ||
      resources_->texture_id_map.HasClientID(texture_client_id)) {
    InsertError(GL_INVALID_OPERATION, "invalid client ID");
    return error::kNoError;
  }

  const Mailbox& mb = Mailbox::FromVolatile(
      *reinterpret_cast<const volatile Mailbox*>(mailbox));
  auto shared_image = group_->shared_image_representation_factory()
                          ->ProduceGLTexturePassthrough(mb);
  if (shared_image == nullptr) {
    DoGenTextures(1, &texture_client_id);
    InsertError(GL_INVALID_OPERATION, "invalid mailbox name.");
    return error::kNoError;
  }

  auto texture = shared_image->GetTexturePassthrough();

  resources_->texture_id_map.RemoveClientID(texture_client_id);
  resources_->texture_id_map.SetIDMapping(texture_client_id,
                                          texture->service_id());
  resources_->texture_object_map.RemoveClientID(texture_client_id);
  resources_->texture_object_map.SetIDMapping(texture_client_id, texture);
  resources_->texture_shared_image_map[texture_client_id] =
      std::move(shared_image);

  return error::kNoError;
}
