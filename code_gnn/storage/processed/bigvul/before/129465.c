void GLES2DecoderImpl::PerformIdleWork() {
  ProcessPendingReadPixels();
  if (!async_pixel_transfer_manager_->NeedsProcessMorePendingTransfers())
    return;
  async_pixel_transfer_manager_->ProcessMorePendingTransfers();
  ProcessFinishedAsyncTransfers();
}
