void BrowserViewRenderer::UpdateRootLayerState(
    const gfx::Vector2dF& total_scroll_offset_dip,
    const gfx::Vector2dF& max_scroll_offset_dip,
    const gfx::SizeF& scrollable_size_dip,
    float page_scale_factor,
    float min_page_scale_factor,
    float max_page_scale_factor) {
  TRACE_EVENT_INSTANT1(
      "android_webview",
      "BrowserViewRenderer::UpdateRootLayerState",
      TRACE_EVENT_SCOPE_THREAD,
      "state",
      RootLayerStateAsValue(total_scroll_offset_dip, scrollable_size_dip));

  DCHECK_GE(max_scroll_offset_dip.x(), 0.f);
  DCHECK_GE(max_scroll_offset_dip.y(), 0.f);
  DCHECK_GT(page_scale_factor, 0.f);
  DCHECK_GT(dip_scale_, 0.f);

  if (max_scroll_offset_dip_ != max_scroll_offset_dip ||
      scrollable_size_dip_ != scrollable_size_dip ||
      page_scale_factor_ != page_scale_factor ||
      min_page_scale_factor_ != min_page_scale_factor ||
      max_page_scale_factor_ != max_page_scale_factor) {
    max_scroll_offset_dip_ = max_scroll_offset_dip;
    scrollable_size_dip_ = scrollable_size_dip;
    page_scale_factor_ = page_scale_factor;
    min_page_scale_factor_ = min_page_scale_factor;
    max_page_scale_factor_ = max_page_scale_factor;

    client_->UpdateScrollState(max_scroll_offset(), scrollable_size_dip,
                               page_scale_factor, min_page_scale_factor,
                               max_page_scale_factor);
  }
  SetTotalRootLayerScrollOffset(total_scroll_offset_dip);
}
