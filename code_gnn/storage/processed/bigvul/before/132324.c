blink::WebEncryptedMediaClient* RenderFrameImpl::encryptedMediaClient() {
  if (!web_encrypted_media_client_) {
    web_encrypted_media_client_.reset(new media::WebEncryptedMediaClientImpl(
        base::Bind(&RenderFrameImpl::AreSecureCodecsSupported,
                   base::Unretained(this)),
        GetCdmFactory(), GetMediaPermission()));
  }
  return web_encrypted_media_client_.get();
}
