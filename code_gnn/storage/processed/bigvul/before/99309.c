void ResourceMessageFilter::OnGetPluginPath(const GURL& url,
                                            const GURL& policy_url,
                                            const std::string& mime_type,
                                            FilePath* filename,
                                            std::string* url_mime_type) {
  *filename = plugin_service_->GetPluginPath(
      url, policy_url, mime_type, url_mime_type);
}
