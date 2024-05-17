BrowserViewRenderer::RootLayerStateAsValue(
    const gfx::Vector2dF& total_scroll_offset_dip,
    const gfx::SizeF& scrollable_size_dip) {
  scoped_refptr<base::trace_event::TracedValue> state =
      new base::trace_event::TracedValue();

  state->SetDouble("total_scroll_offset_dip.x", total_scroll_offset_dip.x());
  state->SetDouble("total_scroll_offset_dip.y", total_scroll_offset_dip.y());

  state->SetDouble("max_scroll_offset_dip.x", max_scroll_offset_dip_.x());
  state->SetDouble("max_scroll_offset_dip.y", max_scroll_offset_dip_.y());

  state->SetDouble("scrollable_size_dip.width", scrollable_size_dip.width());
  state->SetDouble("scrollable_size_dip.height", scrollable_size_dip.height());

  state->SetDouble("page_scale_factor", page_scale_factor_);
   return state;
 }
