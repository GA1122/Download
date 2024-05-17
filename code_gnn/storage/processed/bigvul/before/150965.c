void DevToolsUIBindings::AppendToFile(const std::string& url,
                                      const std::string& content) {
  file_helper_->Append(url, content,
                       base::Bind(&DevToolsUIBindings::AppendedTo,
                                  weak_factory_.GetWeakPtr(), url));
}
