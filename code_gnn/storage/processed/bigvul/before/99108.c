void ResourceDispatcherHost::OnUploadProgressACK(int request_id) {
  int child_id = receiver_->id();
  PendingRequestList::iterator i = pending_requests_.find(
      GlobalRequestID(child_id, request_id));
  if (i == pending_requests_.end())
    return;

  ResourceDispatcherHostRequestInfo* info = InfoForRequest(i->second);
  info->set_waiting_for_upload_progress_ack(false);
}
