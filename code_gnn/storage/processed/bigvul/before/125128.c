bool PluginServiceImpl::GetPluginInfoArray(
    const GURL& url,
    const std::string& mime_type,
    bool allow_wildcard,
    std::vector<webkit::WebPluginInfo>* plugins,
    std::vector<std::string>* actual_mime_types) {
  bool use_stale = false;
  plugin_list_->GetPluginInfoArray(url, mime_type, allow_wildcard,
                                   &use_stale, plugins, actual_mime_types);
  return use_stale;
}
