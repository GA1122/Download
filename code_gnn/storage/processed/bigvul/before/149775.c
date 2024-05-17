void LayerTreeHost::ApplyViewportDeltas(ScrollAndScaleSet* info) {
  gfx::Vector2dF inner_viewport_scroll_delta;
  if (info->inner_viewport_scroll.layer_id != Layer::INVALID_ID)
    inner_viewport_scroll_delta = info->inner_viewport_scroll.scroll_delta;

  if (inner_viewport_scroll_delta.IsZero() && info->page_scale_delta == 1.f &&
      info->elastic_overscroll_delta.IsZero() && !info->top_controls_delta)
    return;

  if (inner_viewport_scroll_layer_) {
    inner_viewport_scroll_layer_->SetScrollOffsetFromImplSide(
        gfx::ScrollOffsetWithDelta(
            inner_viewport_scroll_layer_->scroll_offset(),
            inner_viewport_scroll_delta));
  }

  ApplyPageScaleDeltaFromImplSide(info->page_scale_delta);
  SetElasticOverscrollFromImplSide(elastic_overscroll_ +
                                   info->elastic_overscroll_delta);
  client_->ApplyViewportDeltas(inner_viewport_scroll_delta, gfx::Vector2dF(),
                               info->elastic_overscroll_delta,
                               info->page_scale_delta,
                               info->top_controls_delta);
  SetNeedsUpdateLayers();
}
