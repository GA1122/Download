TestBackgroundLoaderOffliner::GetVisibleSecurityState(
    content::WebContents* web_contents) {
  if (custom_visible_security_state_)
    return std::move(custom_visible_security_state_);
  return BackgroundLoaderOffliner::GetVisibleSecurityState(web_contents);
}
