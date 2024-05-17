void RenderViewImpl::OnResolveTapDisambiguation(double timestamp_seconds,
                                                gfx::Point tap_viewport_offset,
                                                bool is_long_press) {
  webview()->ResolveTapDisambiguation(timestamp_seconds, tap_viewport_offset,
                                      is_long_press);
}
