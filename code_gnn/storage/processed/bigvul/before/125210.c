void RenderMessageFilter::OnGetPluginInfo(
    int routing_id,
    const GURL& url,
    const GURL& page_url,
    const std::string& mime_type,
    bool* found,
    webkit::WebPluginInfo* info,
    std::string* actual_mime_type) {
  bool allow_wildcard = true;
  *found = plugin_service_->GetPluginInfo(
      render_process_id_, routing_id, resource_context_,
      url, page_url, mime_type, allow_wildcard,
      NULL, info, actual_mime_type);
}
