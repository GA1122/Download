void NavigationObserver::OnExtensionUninstalled(
    content::BrowserContext* browser_context,
    const Extension* extension,
    UninstallReason reason) {
  if (in_progress_prompt_extension_id_.empty() ||
      in_progress_prompt_extension_id_ != extension->id()) {
    return;
  }

  in_progress_prompt_extension_id_ = std::string();
  in_progress_prompt_navigation_controller_ = nullptr;
  extension_install_prompt_.reset();
}
