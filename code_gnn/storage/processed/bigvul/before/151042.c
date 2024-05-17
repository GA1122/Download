void DevToolsUIBindings::UpgradeDraggedFileSystemPermissions(
    const std::string& file_system_url) {
  CHECK(web_contents_->GetURL().SchemeIs(content::kChromeDevToolsScheme));
  file_helper_->UpgradeDraggedFileSystemPermissions(
      file_system_url,
      base::Bind(&DevToolsUIBindings::ShowDevToolsConfirmInfoBar,
                 weak_factory_.GetWeakPtr()));
}
