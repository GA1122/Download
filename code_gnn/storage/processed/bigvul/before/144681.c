bool WebContentsImpl::ShouldRouteMessageEvent(
    RenderFrameHost* target_rfh,
    SiteInstance* source_site_instance) const {
  return GetBrowserPluginGuest() || GetBrowserPluginEmbedder();
}
