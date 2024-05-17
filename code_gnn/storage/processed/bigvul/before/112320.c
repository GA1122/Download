void ResourceDispatcherHostImpl::CancelRequestsForProcess(int child_id) {
  CancelRequestsForRoute(child_id, -1  );
  registered_temp_files_.erase(child_id);
}
