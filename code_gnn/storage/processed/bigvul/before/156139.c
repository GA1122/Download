AcceleratedStaticBitmapImage::CreateFromWebGLContextImage(
    const gpu::Mailbox& mailbox,
    const gpu::SyncToken& sync_token,
    unsigned texture_id,
    base::WeakPtr<WebGraphicsContext3DProviderWrapper>&&
        context_provider_wrapper,
    IntSize mailbox_size) {
  return base::AdoptRef(new AcceleratedStaticBitmapImage(
      mailbox, sync_token, texture_id, std::move(context_provider_wrapper),
      mailbox_size));
}
