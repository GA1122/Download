bool PictureLayer::Update(ResourceUpdateQueue* queue,
                          const OcclusionTracker<Layer>* occlusion) {
  update_source_frame_number_ = layer_tree_host()->source_frame_number();
  bool updated = Layer::Update(queue, occlusion);

  bool can_use_lcd_text_changed = UpdateCanUseLCDText();

  gfx::Rect visible_layer_rect = gfx::ScaleToEnclosingRect(
      visible_content_rect(), 1.f / contents_scale_x());
  gfx::Size layer_size = paint_properties().bounds;

  if (last_updated_visible_content_rect_ == visible_content_rect() &&
      recording_source_->GetSize() == layer_size && !can_use_lcd_text_changed &&
      pending_invalidation_.IsEmpty()) {
    return updated;
  }

  TRACE_EVENT1("cc", "PictureLayer::Update",
               "source_frame_number",
               layer_tree_host()->source_frame_number());
  devtools_instrumentation::ScopedLayerTreeTask update_layer(
      devtools_instrumentation::kUpdateLayer, id(), layer_tree_host()->id());

  pending_invalidation_.Swap(&recording_invalidation_);
  pending_invalidation_.Clear();

  if (layer_tree_host()->settings().record_full_layer) {
    visible_layer_rect = gfx::Rect(layer_size);
  }

  DCHECK(client_);
  updated |= recording_source_->UpdateAndExpandInvalidation(
      client_, &recording_invalidation_, can_use_lcd_text_for_update_,
      layer_size, visible_layer_rect, update_source_frame_number_,
      Picture::RECORD_NORMALLY);
  last_updated_visible_content_rect_ = visible_content_rect();

  if (updated) {
    SetNeedsPushProperties();
  } else {
    recording_invalidation_.Clear();
  }

  return updated;
}
