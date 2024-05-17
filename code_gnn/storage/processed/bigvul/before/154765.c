error::Error GLES2DecoderPassthroughImpl::DoProduceTextureDirectCHROMIUM(
    GLuint texture_client_id,
    const volatile GLbyte* mailbox) {
  scoped_refptr<TexturePassthrough> texture = nullptr;
  if (!resources_->texture_object_map.GetServiceID(texture_client_id,
                                                   &texture) ||
      texture == nullptr) {
    InsertError(GL_INVALID_OPERATION, "Unknown texture.");
    return error::kNoError;
  }

  const Mailbox& mb = Mailbox::FromVolatile(
      *reinterpret_cast<const volatile Mailbox*>(mailbox));
  mailbox_manager_->ProduceTexture(mb, texture.get());
  return error::kNoError;
}
