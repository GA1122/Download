void ResourceDispatcherHostImpl::OnReleaseDownloadedFile(int request_id) {
  DCHECK(pending_requests_.end() ==
         pending_requests_.find(
             GlobalRequestID(filter_->child_id(), request_id)));
  UnregisterDownloadedTempFile(filter_->child_id(), request_id);
}
