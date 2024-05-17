bool RenderFrameImpl::IsEncryptedMediaEnabled() const {
  return GetRendererPreferences().enable_encrypted_media;
}
