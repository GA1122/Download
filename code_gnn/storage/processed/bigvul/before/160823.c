void RenderViewImpl::ResizeWebWidget() {
  webview()->ResizeWithBrowserControls(
      GetSizeForWebWidget(), top_controls_height_, bottom_controls_height_,
      browser_controls_shrink_blink_size_);
}
