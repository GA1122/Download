void BrowserView::SetToolbarButtonProvider(ToolbarButtonProvider* provider) {
  DCHECK(!toolbar_button_provider_);
  toolbar_button_provider_ = provider;
}
