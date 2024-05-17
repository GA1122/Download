void ResourceDispatcherHost::StartDeferredRequest(int process_unique_id,
                                                  int request_id) {
  GlobalRequestID global_id(process_unique_id, request_id);
  PendingRequestList::iterator i = pending_requests_.find(global_id);
  if (i == pending_requests_.end()) {
    LOG(WARNING) << "Trying to resume a non-existent request ("
                 << process_unique_id << ", " << request_id << ")";
    return;
  }


  URLRequest* request = i->second;
  InsertIntoResourceQueue(request, *InfoForRequest(request));
}
