void ResourceDispatcherHost::CancelRequest(int child_id,
                                           int request_id,
                                           bool from_renderer) {
  CancelRequest(child_id, request_id, from_renderer, true);
}
