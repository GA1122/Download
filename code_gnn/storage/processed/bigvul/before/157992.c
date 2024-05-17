void RenderViewImpl::SetScreenMetricsEmulationParametersForWidget(
    bool enabled,
    const blink::WebDeviceEmulationParams& params) {
  if (webview() && GetWidget()->layer_tree_view()) {
    if (enabled)
      webview()->EnableDeviceEmulation(params);
    else
      webview()->DisableDeviceEmulation();
  }
}
