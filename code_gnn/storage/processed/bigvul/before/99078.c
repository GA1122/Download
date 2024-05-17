void ResourceDispatcherHost::DataReceivedACK(int child_id,
                                             int request_id) {
  PendingRequestList::iterator i = pending_requests_.find(
      GlobalRequestID(child_id, request_id));
  if (i == pending_requests_.end())
    return;

  ResourceDispatcherHostRequestInfo* info = InfoForRequest(i->second);

  info->DecrementPendingDataCount();

  if (info->pending_data_count() == kMaxPendingDataMessages) {
    info->DecrementPendingDataCount();

    PauseRequest(child_id, request_id, false);
  }
}
