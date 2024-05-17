bool GLES2DecoderImpl::HasMoreIdleWork() {
  return !pending_readpixel_fences_.empty() ||
      async_pixel_transfer_manager_->NeedsProcessMorePendingTransfers();
}
