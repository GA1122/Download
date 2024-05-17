bool NavigationControllerImpl::CanViewSource() const {
  const std::string& mime_type = delegate_->GetContentsMimeType();
  bool is_viewable_mime_type = blink::IsSupportedNonImageMimeType(mime_type) &&
                               !media::IsSupportedMediaMimeType(mime_type);
  NavigationEntry* visible_entry = GetVisibleEntry();
  return visible_entry && !visible_entry->IsViewSourceMode() &&
      is_viewable_mime_type && !delegate_->GetInterstitialPage();
}
