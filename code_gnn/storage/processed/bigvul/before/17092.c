OxideQQuickWebView::editingCapabilities() const {
  Q_D(const OxideQQuickWebView);

  if (!d->proxy_) {
    return NoCapability;
  }

  oxide::qt::EditCapabilityFlags flags = d->proxy_->editFlags();
  return static_cast<EditCapabilities>(flags);
}
