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

  DCHECK_GT(dip_scale_, 0.f);

  max_scroll_offset_dip_ = max_scroll_offset_dip;
  DCHECK_LE(0.f, max_scroll_offset_dip_.x());
  DCHECK_LE(0.f, max_scroll_offset_dip_.y());

  page_scale_factor_ = page_scale_factor;
  DCHECK_GT(page_scale_factor_, 0.f);

  client_->UpdateScrollState(max_scroll_offset(),
                             scrollable_size_dip,
                             page_scale_factor,
                             min_page_scale_factor,
                             max_page_scale_factor);
  SetTotalRootLayerScrollOffset(total_scroll_offset_dip);
}
