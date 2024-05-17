bool RenderFrameImpl::SwapIn() {
  CHECK_NE(previous_routing_id_, MSG_ROUTING_NONE);
  CHECK(!in_frame_tree_);

  RenderFrameProxy* proxy =
      RenderFrameProxy::FromRoutingID(previous_routing_id_);
  CHECK(proxy);

  unique_name_helper_.set_propagated_name(proxy->unique_name());

  if (!proxy->web_frame()->Swap(frame_)) {
    DCHECK(!is_main_frame_);
    return false;
  }

  previous_routing_id_ = MSG_ROUTING_NONE;
  in_frame_tree_ = true;

  if (is_main_frame_) {
    CHECK(!render_view_->main_render_frame_);
    render_view_->main_render_frame_ = this;
    if (render_view_->GetWidget()->is_frozen()) {
      render_view_->GetWidget()->SetIsFrozen(false);
    }
    render_view_->GetWidget()->UpdateWebViewWithDeviceScaleFactor();

    render_view_->webview()->DidAttachLocalMainFrame(render_view_->GetWidget());
  }

  return true;
}
