void WebContentsImpl::RenderWidgetLostFocus(
    RenderWidgetHostImpl* render_widget_host) {
  NotifyWebContentsLostFocus();
}
