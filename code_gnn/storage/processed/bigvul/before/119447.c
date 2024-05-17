ScreenOrientationDispatcherHost::GetRenderFrameHostForRequestID(
    int request_id) {
  if (!current_lock_ || current_lock_->request_id != request_id)
    return NULL;

  return RenderFrameHost::FromID(current_lock_->process_id,
                                 current_lock_->routing_id);
}
