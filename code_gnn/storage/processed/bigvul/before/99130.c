bool ResourceDispatcherHost::WillSendData(int child_id,
                                          int request_id) {
  PendingRequestList::iterator i = pending_requests_.find(
      GlobalRequestID(child_id, request_id));
  if (i == pending_requests_.end()) {
    NOTREACHED() << "WillSendData for invalid request";
    return false;
  }

  ResourceDispatcherHostRequestInfo* info = InfoForRequest(i->second);

  info->IncrementPendingDataCount();
  if (info->pending_data_count() > kMaxPendingDataMessages) {
    PauseRequest(child_id, request_id, true);
    return false;
  }

  return true;
}
