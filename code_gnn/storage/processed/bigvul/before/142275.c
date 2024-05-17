void ChromePasswordManagerClient::DidStartNavigation(
    content::NavigationHandle* navigation_handle) {
  log_manager_->SetSuspended(web_contents()->GetWebUI() != nullptr);
}
