void GLES2DecoderImpl::DoProduceTextureDirectCHROMIUM(
    GLuint client_id,
    const volatile GLbyte* data) {
  TRACE_EVENT2("gpu", "GLES2DecoderImpl::DoProduceTextureDirectCHROMIUM",
      "context", logger_.GetLogPrefix(),
      "mailbox[0]", static_cast<unsigned char>(data[0]));
  Mailbox mailbox =
      Mailbox::FromVolatile(*reinterpret_cast<const volatile Mailbox*>(data));
  DLOG_IF(ERROR, !mailbox.Verify())
      << "ProduceTextureDirectCHROMIUM was not passed a crypto-random mailbox.";

  TextureRef* texture_ref = GetTexture(client_id);
  if (!texture_ref) {
    LOCAL_SET_GL_ERROR(GL_INVALID_OPERATION, "glProduceTextureDirectCHROMIUM",
                       "unknown texture");
    return;
  }

  group_->mailbox_manager()->ProduceTexture(mailbox, texture_ref->texture());
}
