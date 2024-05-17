void ResourceDispatcherHostImpl::OnDataReceivedACK(int request_id) {
  DataReceivedACK(filter_->child_id(), request_id);
}
