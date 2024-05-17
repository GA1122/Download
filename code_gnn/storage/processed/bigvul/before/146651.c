PassRefPtr<StaticBitmapImage> DrawingBuffer::TransferToStaticBitmapImage() {
  ScopedStateRestorer scoped_state_restorer(this);

  GrContext* gr_context = ContextProvider()->GetGrContext();

  viz::TextureMailbox texture_mailbox;
  std::unique_ptr<cc::SingleReleaseCallback> release_callback;
  bool success = false;
  if (gr_context) {
    bool force_gpu_result = true;
    success = PrepareTextureMailboxInternal(&texture_mailbox, &release_callback,
                                            force_gpu_result);
  }
  if (!success) {
    sk_sp<SkSurface> surface =
        SkSurface::MakeRasterN32Premul(size_.Width(), size_.Height());
    return StaticBitmapImage::Create(surface->makeImageSnapshot());
  }

  DCHECK_EQ(size_.Width(), texture_mailbox.size_in_pixels().width());
  DCHECK_EQ(size_.Height(), texture_mailbox.size_in_pixels().height());

  GLuint texture_id = gl_->CreateAndConsumeTextureCHROMIUM(
      GL_TEXTURE_2D, texture_mailbox.name());
  release_callback->Run(gpu::SyncToken(), true  );

  const auto& sk_image_mailbox = texture_mailbox.mailbox();
  const auto& sk_image_sync_token = texture_mailbox.sync_token();

  return AcceleratedStaticBitmapImage::CreateFromWebGLContextImage(
      sk_image_mailbox, sk_image_sync_token, texture_id,
      context_provider_->CreateWeakPtr(), size_);
}
