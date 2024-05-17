void RenderViewImpl::SetFocus(bool enable) {
  RenderWidget::OnSetFocus(enable);

  if (BrowserPluginManager::Get())
    BrowserPluginManager::Get()->UpdateFocusState();
}
