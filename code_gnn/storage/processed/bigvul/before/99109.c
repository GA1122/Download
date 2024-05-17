void ResourceDispatcherHost::PauseRequest(int child_id,
                                          int request_id,
                                          bool pause) {
  GlobalRequestID global_id(child_id, request_id);
  PendingRequestList::iterator i = pending_requests_.find(global_id);
  if (i == pending_requests_.end()) {
    DLOG(WARNING) << "Pausing a request that wasn't found";
    return;
  }

  ResourceDispatcherHostRequestInfo* info = InfoForRequest(i->second);
  int pause_count = info->pause_count() + (pause ? 1 : -1);
  if (pause_count < 0) {
    NOTREACHED();   
    return;
  }
  info->set_pause_count(pause_count);

  RESOURCE_LOG("To pause (" << pause << "): " << i->second->url().spec());

  if (info->pause_count() == 0) {
    MessageLoop::current()->PostTask(FROM_HERE,
        method_runner_.NewRunnableMethod(
            &ResourceDispatcherHost::ResumeRequest, global_id));
  }
}
