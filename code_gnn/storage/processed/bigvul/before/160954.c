void ChromeClientImpl::DispatchViewportPropertiesDidChange(
    const ViewportDescription& description) const {
  web_view_->UpdatePageDefinedViewportConstraints(description);
}
