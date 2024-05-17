void DevToolsUIBindings::AddFileSystem(const std::string& file_system_path) {
  CHECK(web_contents_->GetURL().SchemeIs(content::kChromeDevToolsScheme));
  file_helper_->AddFileSystem(
      file_system_path,
      base::Bind(&DevToolsUIBindings::ShowDevToolsConfirmInfoBar,
                 weak_factory_.GetWeakPtr()));
}
