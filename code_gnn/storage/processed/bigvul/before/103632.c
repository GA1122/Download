void ChromeContentBrowserClient::UpdateInspectorSetting(
    RenderViewHost* rvh, const std::string& key, const std::string& value) {
  RenderViewHostDelegateHelper::UpdateInspectorSetting(
      rvh->process()->profile(), key, value);
}
