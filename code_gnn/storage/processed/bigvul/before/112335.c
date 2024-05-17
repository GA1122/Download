ResourceLoader* ResourceDispatcherHostImpl::GetLoader(int child_id,
                                                      int request_id) const {
  return GetLoader(GlobalRequestID(child_id, request_id));
}
