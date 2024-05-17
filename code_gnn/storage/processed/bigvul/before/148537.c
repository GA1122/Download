void WebContentsImpl::RendererUnresponsive(
    RenderWidgetHostImpl* render_widget_host) {
  for (auto& observer : observers_)
    observer.OnRendererUnresponsive(render_widget_host);

  if (render_widget_host != GetRenderViewHost()->GetWidget())
    return;

  if (ShouldIgnoreUnresponsiveRenderer())
    return;

  if (!GetRenderViewHost() || !GetRenderViewHost()->IsRenderViewLive())
    return;

  if (delegate_) {
    WebContentsUnresponsiveState unresponsive_state;
    unresponsive_state.outstanding_ack_count =
        render_widget_host->in_flight_event_count();
    unresponsive_state.outstanding_event_type =
        render_widget_host->hang_monitor_event_type();
    unresponsive_state.last_event_type = render_widget_host->last_event_type();
    delegate_->RendererUnresponsive(this, unresponsive_state);
  }
}
