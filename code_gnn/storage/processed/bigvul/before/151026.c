void DevToolsUIBindings::SaveToFile(const std::string& url,
                                    const std::string& content,
                                    bool save_as) {
  file_helper_->Save(url, content, save_as,
                     base::Bind(&DevToolsUIBindings::FileSavedAs,
                                weak_factory_.GetWeakPtr(), url),
                     base::Bind(&DevToolsUIBindings::CanceledFileSaveAs,
                                weak_factory_.GetWeakPtr(), url));
}
