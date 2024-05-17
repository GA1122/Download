void FetchManager::Loader::Dispose() {
  probe::detachClientRequest(execution_context_, this);
  fetch_manager_ = nullptr;
  if (threadable_loader_) {
    if (fetch_request_data_->Keepalive())
      threadable_loader_->Detach();
    else
      threadable_loader_->Cancel();
    threadable_loader_ = nullptr;
  }
  if (integrity_verifier_)
    integrity_verifier_->Cancel();
  execution_context_ = nullptr;
}
