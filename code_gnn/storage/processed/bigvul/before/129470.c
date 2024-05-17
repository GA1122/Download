void GLES2DecoderImpl::ProcessFinishedAsyncTransfers() {
  ProcessPendingReadPixels();
  if (engine() && query_manager_.get())
    query_manager_->ProcessPendingTransferQueries();

  async_pixel_transfer_manager_->BindCompletedAsyncTransfers();
}
