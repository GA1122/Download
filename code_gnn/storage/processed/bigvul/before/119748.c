bool NavigationControllerImpl::CanViewSource() const {
  const std::string& mime_type = web_contents_->GetContentsMimeType();
  bool is_viewable_mime_type = net::IsSupportedNonImageMimeType(mime_type) &&
      !net::IsSupportedMediaMimeType(mime_type);
  NavigationEntry* active_entry = GetActiveEntry();
  return active_entry && !active_entry->IsViewSourceMode() &&
      is_viewable_mime_type && !web_contents_->GetInterstitialPage();
}
