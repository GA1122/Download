void ResourceDispatcherHostImpl::ResumeDeferredRequest(int child_id,
                                                       int request_id) {
  PauseRequest(child_id, request_id, false);
}
