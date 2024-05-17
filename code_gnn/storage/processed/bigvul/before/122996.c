void RenderWidgetHostImpl::NotifyTextDirection() {
  if (text_direction_updated_) {
    if (!text_direction_canceled_)
      Send(new ViewMsg_SetTextDirection(GetRoutingID(), text_direction_));
    text_direction_updated_ = false;
    text_direction_canceled_ = false;
  }
}
