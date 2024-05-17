bool ValidatePluginChildId(int plugin_child_id) {
  if (plugin_child_id == ChildProcessHost::kInvalidUniqueID)
    return true;

#if BUILDFLAG(ENABLE_PLUGINS)
  auto* plugin_host = BrowserChildProcessHost::FromID(plugin_child_id);
  if (plugin_host) {
    int process_type = plugin_host->GetData().process_type;
    if (process_type == PROCESS_TYPE_PPAPI_PLUGIN) {
      return true;
    } else if (process_type >= PROCESS_TYPE_CONTENT_END) {
      if (GetContentClient()->browser()->GetExternalBrowserPpapiHost(
              plugin_child_id) != nullptr) {
        return true;
      }
    }
  }
#endif
  return false;
}
