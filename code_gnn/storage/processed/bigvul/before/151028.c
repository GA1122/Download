void DevToolsUIBindings::SearchInPath(int search_request_id,
                                      const std::string& file_system_path,
                                      const std::string& query) {
  DCHECK_CURRENTLY_ON(BrowserThread::UI);
  CHECK(web_contents_->GetURL().SchemeIs(content::kChromeDevToolsScheme));
  if (!file_helper_->IsFileSystemAdded(file_system_path)) {
    SearchCompleted(search_request_id,
                    file_system_path,
                    std::vector<std::string>());
    return;
  }
  file_system_indexer_->SearchInPath(file_system_path,
                                     query,
                                     Bind(&DevToolsUIBindings::SearchCompleted,
                                          weak_factory_.GetWeakPtr(),
                                          search_request_id,
                                          file_system_path));
}
