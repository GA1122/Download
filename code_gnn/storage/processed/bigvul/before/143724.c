void RenderWidgetHostImpl::WasResized() {
  if (resize_ack_pending_ || !process_->HasConnection() || !view_ ||
      !renderer_initialized_ || auto_resize_enabled_ || !delegate_) {
    if (resize_ack_pending_ && color_profile_out_of_date_)
      DispatchColorProfile();
    return;
  }

  scoped_ptr<ResizeParams> params(new ResizeParams);
  if (color_profile_out_of_date_)
    DispatchColorProfile();
  if (!GetResizeParams(params.get()))
    return;

  bool width_changed =
      !old_resize_params_ ||
      old_resize_params_->new_size.width() != params->new_size.width();
  if (Send(new ViewMsg_Resize(routing_id_, *params))) {
    resize_ack_pending_ = params->needs_resize_ack;
    old_resize_params_.swap(params);
  }

  if (delegate_)
    delegate_->RenderWidgetWasResized(this, width_changed);
}
