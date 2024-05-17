base::WeakPtr<InFlightBackendIO> BackendImpl::GetBackgroundQueue() {
  return background_queue_.GetWeakPtr();
}
