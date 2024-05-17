void WebContentsImpl::RenderWidgetWasResized(
    RenderWidgetHostImpl* render_widget_host,
    bool width_changed) {
  RenderFrameHostImpl* rfh = GetMainFrame();
  if (!rfh || render_widget_host != rfh->GetRenderWidgetHost())
    return;

  FOR_EACH_OBSERVER(WebContentsObserver, observers_,
                    MainFrameWasResized(width_changed));
}
