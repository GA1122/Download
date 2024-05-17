void WebContentsImpl::RenderWidgetWasResized(
    RenderWidgetHostImpl* render_widget_host,
    bool width_changed) {
  RenderFrameHostImpl* rfh = GetMainFrame();
  if (!rfh || render_widget_host != rfh->GetRenderWidgetHost())
    return;

  ScreenInfo screen_info;
  GetScreenInfo(&screen_info);
  SendPageMessage(new PageMsg_UpdateScreenInfo(MSG_ROUTING_NONE, screen_info));

  for (auto& observer : observers_)
    observer.MainFrameWasResized(width_changed);
}
