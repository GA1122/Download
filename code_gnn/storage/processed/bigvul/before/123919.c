void RenderViewImpl::OnUpdateTimezone() {
  if (webview())
    NotifyTimezoneChange(webview()->mainFrame());
}
