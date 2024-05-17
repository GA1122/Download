void ChromeContentBrowserClient::GuestPermissionRequestHelper(
    const GURL& url,
    const std::vector<content::GlobalFrameRoutingId>& render_frames,
    base::Callback<void(bool)> callback,
    bool allow) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  std::map<int, int> process_map;
  std::map<int, int>::const_iterator it;
  bool has_web_view_guest = false;
  for (const auto& it : render_frames) {
    if (process_map.find(it.child_id) != process_map.end())
      continue;

    process_map.insert(std::pair<int, int>(it.child_id, it.frame_routing_id));

    if (extensions::WebViewRendererState::GetInstance()->IsGuest(it.child_id))
      has_web_view_guest = true;
  }
  if (!has_web_view_guest) {
    FileSystemAccessed(url, render_frames, callback, allow);
    return;
  }
  DCHECK_EQ(1U, process_map.size());
  it = process_map.begin();
  base::PostTaskWithTraits(
      FROM_HERE, {BrowserThread::UI},
      base::BindOnce(
          &ChromeContentBrowserClient::RequestFileSystemPermissionOnUIThread,
          it->first, it->second, url, allow,
          base::Bind(
              &ChromeContentBrowserClient::FileSystemAccessed,
              weak_factory_.GetWeakPtr(), url, render_frames,
              base::Bind(&InvokeCallbackOnThread,
                         base::SequencedTaskRunnerHandle::Get(), callback))));
}
