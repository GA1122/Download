bool RenderViewImpl::GetPluginInfo(const GURL& url,
                                   const GURL& page_url,
                                   const std::string& mime_type,
                                   webkit::WebPluginInfo* plugin_info,
                                   std::string* actual_mime_type) {
  bool found = false;
  Send(new ViewHostMsg_GetPluginInfo(
      routing_id_, url, page_url, mime_type, &found, plugin_info,
      actual_mime_type));
  return found;
}
