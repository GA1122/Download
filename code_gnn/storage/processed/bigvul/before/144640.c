void WebContentsImpl::RenderWidgetDeleted(
    RenderWidgetHostImpl* render_widget_host) {
  created_widgets_.erase(render_widget_host);

  if (is_being_destroyed_)
    return;

  if (render_widget_host &&
      render_widget_host->GetRoutingID() == fullscreen_widget_routing_id_) {
    if (delegate_ && delegate_->EmbedsFullscreenWidget())
      delegate_->ExitFullscreenModeForTab(this);
    FOR_EACH_OBSERVER(WebContentsObserver,
                      observers_,
                      DidDestroyFullscreenWidget(
                          fullscreen_widget_routing_id_));
    fullscreen_widget_routing_id_ = MSG_ROUTING_NONE;
    if (fullscreen_widget_had_focus_at_shutdown_)
      view_->RestoreFocus();
  }
}
