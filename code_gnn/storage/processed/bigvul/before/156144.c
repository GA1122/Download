void AcceleratedStaticBitmapImage::UpdateSyncToken(gpu::SyncToken sync_token) {
  texture_holder_->UpdateSyncToken(sync_token);
}
