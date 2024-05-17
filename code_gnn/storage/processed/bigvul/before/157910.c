void RenderViewImpl::DidChangeFocusForWidget() {
  if (BrowserPluginManager::Get())
    BrowserPluginManager::Get()->UpdateFocusState();
}
