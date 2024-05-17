void ResourceDispatcherHost::OnDataReceivedACK(int request_id) {
  DataReceivedACK(receiver_->id(), request_id);
}
