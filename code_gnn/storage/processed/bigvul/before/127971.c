gfx::Vector2d BrowserViewRenderer::max_scroll_offset() const {
  DCHECK_GT(dip_scale_, 0.f);
  return gfx::ToCeiledVector2d(gfx::ScaleVector2d(
       max_scroll_offset_dip_, dip_scale_ * page_scale_factor_));
 }
