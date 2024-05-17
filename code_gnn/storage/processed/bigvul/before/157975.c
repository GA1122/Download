void RenderViewImpl::ResizeWebWidgetForWidget(
    const gfx::Size& size,
    float top_controls_height,
    float bottom_controls_height,
    bool browser_controls_shrink_blink_size) {
  webview()->ResizeWithBrowserControls(size, top_controls_height,
                                       bottom_controls_height,
                                       browser_controls_shrink_blink_size);
}
