void RenderFrameImpl::DidReceiveTransferSizeUpdate(int resource_id,
                                                   int received_data_length) {
  for (auto& observer : observers_) {
    observer.DidReceiveTransferSizeUpdate(resource_id, received_data_length);
  }
}
