WebDevToolsAgent* DevToolsAgent::GetWebAgent() {
  WebView* web_view = render_view()->webview();
  if (!web_view)
    return NULL;
  return web_view->devToolsAgent();
}
