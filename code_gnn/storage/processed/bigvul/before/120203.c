void LayerTreeHost::ApplyScrollAndScale(const ScrollAndScaleSet& info) {
  if (!root_layer_.get())
    return;

  gfx::Vector2d inner_viewport_scroll_delta;
  gfx::Vector2d outer_viewport_scroll_delta;

  for (size_t i = 0; i < info.scrolls.size(); ++i) {
    Layer* layer =
        LayerTreeHostCommon::FindLayerInSubtree(root_layer_.get(),
                                                info.scrolls[i].layer_id);
    if (!layer)
      continue;
    if (layer == outer_viewport_scroll_layer_.get()) {
      outer_viewport_scroll_delta += info.scrolls[i].scroll_delta;
    } else if (layer == inner_viewport_scroll_layer_.get()) {
      inner_viewport_scroll_delta += info.scrolls[i].scroll_delta;
    } else {
      layer->SetScrollOffsetFromImplSide(layer->scroll_offset() +
                                         info.scrolls[i].scroll_delta);
    }
  }

  if (!inner_viewport_scroll_delta.IsZero() ||
      !outer_viewport_scroll_delta.IsZero() || info.page_scale_delta != 1.f) {

    DCHECK(inner_viewport_scroll_layer_);   

    inner_viewport_scroll_layer_->SetScrollOffsetFromImplSide(
        inner_viewport_scroll_layer_->scroll_offset() +
        inner_viewport_scroll_delta);
    if (outer_viewport_scroll_layer_) {
      outer_viewport_scroll_layer_->SetScrollOffsetFromImplSide(
          outer_viewport_scroll_layer_->scroll_offset() +
          outer_viewport_scroll_delta);
    }
    ApplyPageScaleDeltaFromImplSide(info.page_scale_delta);

    client_->ApplyScrollAndScale(
        inner_viewport_scroll_delta + outer_viewport_scroll_delta,
        info.page_scale_delta);
   }
 }
