void RenderWidgetHostImpl::DidNavigate(uint32_t next_source_id) {
  current_content_source_id_ = next_source_id;

  if (enable_surface_synchronization_) {
    if (view_)
      view_->DidNavigate();
    next_resize_needs_resize_ack_ = resize_ack_pending_;
    resize_ack_pending_ = false;
    WasResized();
  } else {
    if (last_received_content_source_id_ >= current_content_source_id_)
      return;
  }

  if (!new_content_rendering_timeout_)
    return;

  new_content_rendering_timeout_->Start(new_content_rendering_delay_);
}
