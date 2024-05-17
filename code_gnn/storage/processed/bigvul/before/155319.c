void ChromeContentBrowserClient::FileSystemAccessed(
    const GURL& url,
    const std::vector<content::GlobalFrameRoutingId>& render_frames,
    base::Callback<void(bool)> callback,
    bool allow) {
  for (const auto& it : render_frames) {
    base::PostTaskWithTraits(
        FROM_HERE, {BrowserThread::UI},
        base::BindOnce(&TabSpecificContentSettings::FileSystemAccessed,
                       it.child_id, it.frame_routing_id, url, !allow));
  }
  callback.Run(allow);
}
