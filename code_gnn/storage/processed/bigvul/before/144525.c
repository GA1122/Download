RenderFrameHost* WebContentsImpl::GetGuestByInstanceID(
    RenderFrameHost* render_frame_host,
    int browser_plugin_instance_id) {
  BrowserPluginGuestManager* guest_manager =
      GetBrowserContext()->GetGuestManager();
  if (!guest_manager)
    return nullptr;

  WebContents* guest = guest_manager->GetGuestByInstanceID(
      render_frame_host->GetProcess()->GetID(), browser_plugin_instance_id);
  if (!guest)
    return nullptr;

  return guest->GetMainFrame();
}
