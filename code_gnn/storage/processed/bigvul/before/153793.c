void GLES2Implementation::ProduceTextureDirectCHROMIUM(GLuint texture,
                                                       GLbyte* data) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glProduceTextureDirectCHROMIUM("
                     << static_cast<const void*>(data) << ")");
  static_assert(std::is_trivially_copyable<Mailbox>::value,
                "gpu::Mailbox is not trivially copyable");
  Mailbox result = Mailbox::Generate();
  memcpy(data, result.name, sizeof(result.name));
  helper_->ProduceTextureDirectCHROMIUMImmediate(texture, data);
  CheckGLError();
}
