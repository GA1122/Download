void RenderViewImpl::ApplyNewDisplayModeForWidget(
    const blink::WebDisplayMode& new_display_mode) {
  if (webview())
    webview()->SetDisplayMode(new_display_mode);
}
