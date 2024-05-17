RenderWidgetFullscreenPepper* RenderViewImpl::CreatePepperFullscreenContainer(
    webkit::ppapi::PluginInstance* plugin) {
  GURL active_url;
  if (webview() && webview()->mainFrame())
    active_url = GURL(webview()->mainFrame()->document().url());
  RenderWidgetFullscreenPepper* widget = RenderWidgetFullscreenPepper::Create(
      routing_id_, plugin, active_url, screen_info_);
  widget->show(WebKit::WebNavigationPolicyIgnore);
  return widget;
}
