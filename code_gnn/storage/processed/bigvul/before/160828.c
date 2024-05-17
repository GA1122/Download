void RenderViewImpl::SetScreenMetricsEmulationParameters(
    bool enabled,
    const blink::WebDeviceEmulationParams& params) {
  if (webview() && compositor()) {
    if (enabled)
      webview()->EnableDeviceEmulation(params);
    else
      webview()->DisableDeviceEmulation();
  }
}
