void WebContentsImpl::RenderWidgetCreated(
    RenderWidgetHostImpl* render_widget_host) {
  created_widgets_.insert(render_widget_host);
}
