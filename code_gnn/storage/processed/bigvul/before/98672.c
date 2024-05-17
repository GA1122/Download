void RenderWidget::show(WebNavigationPolicy) {
  DCHECK(!did_show_) << "received extraneous Show call";
  DCHECK(routing_id_ != MSG_ROUTING_NONE);
  DCHECK(opener_id_ != MSG_ROUTING_NONE);

  if (!did_show_) {
    did_show_ = true;
    if (popup_params_.get()) {
      popup_params_->bounds = initial_pos_;
      Send(new ViewHostMsg_ShowPopup(routing_id_, *popup_params_));
      popup_params_.reset();
    } else {
      Send(new ViewHostMsg_ShowWidget(opener_id_, routing_id_, initial_pos_));
    }
    SetPendingWindowRect(initial_pos_);
  }
}
