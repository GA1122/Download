RenderWidgetFullscreenPepper* RenderView::CreatePepperFullscreenContainer(
    webkit::ppapi::PluginInstance* plugin) {
  GURL active_url;
  if (webview() && webview()->mainFrame())
    active_url = GURL(webview()->mainFrame()->document().url());
  RenderWidgetFullscreenPepper* widget = RenderWidgetFullscreenPepper::Create(
      routing_id_, render_thread_, plugin, active_url);
  widget->show(WebKit::WebNavigationPolicyIgnore);
  return widget;
}
