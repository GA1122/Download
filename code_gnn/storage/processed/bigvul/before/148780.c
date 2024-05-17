 RendererPreferences InterstitialPageImpl::GetRendererPrefs(
     BrowserContext* browser_context) const {
   delegate_->OverrideRendererPrefs(&renderer_preferences_);
  return renderer_preferences_;
}
