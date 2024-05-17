void DevToolsUIBindings::RemoveFileSystem(const std::string& file_system_path) {
  CHECK(web_contents_->GetURL().SchemeIs(content::kChromeDevToolsScheme));
  file_helper_->RemoveFileSystem(file_system_path);
}
