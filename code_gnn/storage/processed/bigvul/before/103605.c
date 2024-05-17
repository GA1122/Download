void ChromeContentBrowserClient::ClearInspectorSettings(RenderViewHost* rvh) {
  RenderViewHostDelegateHelper::ClearInspectorSettings(
      rvh->process()->profile());
}
