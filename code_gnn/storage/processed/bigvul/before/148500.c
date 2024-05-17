void WebContentsImpl::OnPluginCrashed(RenderFrameHostImpl* source,
                                      const base::FilePath& plugin_path,
                                      base::ProcessId plugin_pid) {
  for (auto& observer : observers_)
    observer.PluginCrashed(plugin_path, plugin_pid);
}
