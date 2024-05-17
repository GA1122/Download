void WebContentsImpl::SetZoomLevel(double level) {
  Send(new ViewMsg_SetZoomLevel(GetRoutingID(), level));
  BrowserPluginEmbedder* embedder = GetBrowserPluginEmbedder();
  if (embedder)
    embedder->SetZoomLevel(level);
}
