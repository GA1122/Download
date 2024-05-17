void ChromeContentBrowserClient::RenderViewHostCreated(
    RenderViewHost* render_view_host) {
  new ChromeRenderViewHostObserver(render_view_host);
  new DevToolsHandler(render_view_host);
  new ExtensionMessageHandler(render_view_host);

  InitRenderViewHostForExtensions(render_view_host);
}
