void DocumentLoader::DetachFromFrame() {
  DCHECK(frame_);

  fetcher_->StopFetching();

  if (frame_ && !SentDidFinishLoad())
    LoadFailed(ResourceError::CancelledError(Url()));

  if (!frame_)
    return;

  fetcher_->ClearContext();
  application_cache_host_->DetachFromDocumentLoader();
  application_cache_host_.Clear();
  service_worker_network_provider_ = nullptr;
  WeakIdentifierMap<DocumentLoader>::NotifyObjectDestroyed(this);
  ClearMainResourceHandle();
  frame_ = nullptr;
}
