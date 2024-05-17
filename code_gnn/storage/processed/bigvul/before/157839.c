void WebContentsImpl::RenderWidgetWasResized(
    RenderWidgetHostImpl* render_widget_host,
    const ScreenInfo& screen_info,
    bool width_changed) {
  RenderFrameHostImpl* rfh = GetMainFrame();
  if (!rfh || render_widget_host != rfh->GetRenderWidgetHost())
    return;

  SendPageMessage(new PageMsg_UpdateScreenInfo(MSG_ROUTING_NONE, screen_info));

  for (auto& observer : observers_)
    observer.MainFrameWasResized(width_changed);
}
