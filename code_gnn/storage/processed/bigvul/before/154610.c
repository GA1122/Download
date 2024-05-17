error::Error GLES2DecoderPassthroughImpl::DoCreateAndConsumeTextureINTERNAL(
    GLuint texture_client_id,
    const volatile GLbyte* mailbox) {
  if (!texture_client_id ||
      resources_->texture_id_map.HasClientID(texture_client_id)) {
    return error::kInvalidArguments;
  }

  const Mailbox& mb = Mailbox::FromVolatile(
      *reinterpret_cast<const volatile Mailbox*>(mailbox));
  scoped_refptr<TexturePassthrough> texture = TexturePassthrough::CheckedCast(
      group_->mailbox_manager()->ConsumeTexture(mb));
  if (texture == nullptr) {
    DoGenTextures(1, &texture_client_id);
    InsertError(GL_INVALID_OPERATION, "Invalid mailbox name.");
    return error::kNoError;
  }

  resources_->texture_id_map.RemoveClientID(texture_client_id);
  resources_->texture_id_map.SetIDMapping(texture_client_id,
                                          texture->service_id());
  resources_->texture_object_map.RemoveClientID(texture_client_id);
  resources_->texture_object_map.SetIDMapping(texture_client_id, texture);

  return error::kNoError;
}
