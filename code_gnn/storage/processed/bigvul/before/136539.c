void PaintController::ProcessNewItem(DisplayItem& display_item) {
  DCHECK(!construction_disabled_);

  if (IsSkippingCache())
    display_item.SetSkippedCache();

  if (raster_invalidation_tracking_info_) {
    raster_invalidation_tracking_info_->new_client_debug_names.insert(
        &display_item.Client(), display_item.Client().DebugName());
  }

  if (RuntimeEnabledFeatures::SlimmingPaintV175Enabled()) {
    size_t last_chunk_index = new_paint_chunks_.LastChunkIndex();
    bool chunk_added =
        new_paint_chunks_.IncrementDisplayItemIndex(display_item);
    auto& last_chunk = new_paint_chunks_.LastChunk();

#if DCHECK_IS_ON()
    if (chunk_added && last_chunk.is_cacheable) {
      AddToIndicesByClientMap(last_chunk.id.client,
                              new_paint_chunks_.LastChunkIndex(),
                              new_paint_chunk_indices_by_client_);
    }
#endif

    if (chunk_added && last_chunk_index != kNotFound) {
      DCHECK(last_chunk_index != new_paint_chunks_.LastChunkIndex());
      GenerateRasterInvalidations(
          new_paint_chunks_.PaintChunkAt(last_chunk_index));
    }

    last_chunk.outset_for_raster_effects =
        std::max(last_chunk.outset_for_raster_effects,
                 display_item.OutsetForRasterEffects());
  }

#if DCHECK_IS_ON()
  if (new_display_item_list_.size() >= 2 && display_item.IsEnd()) {
    const auto& begin_display_item =
        new_display_item_list_[new_display_item_list_.size() - 2];
    if (begin_display_item.IsBegin() && !begin_display_item.DrawsContent())
      DCHECK(!display_item.IsEndAndPairedWith(begin_display_item.GetType()));
  }

  if (display_item.IsCacheable()) {
    size_t index = FindMatchingItemFromIndex(
        display_item.GetId(), new_display_item_indices_by_client_,
        new_display_item_list_);
    if (index != kNotFound) {
      ShowDebugData();
      NOTREACHED()
          << "DisplayItem " << display_item.AsDebugString().Utf8().data()
          << " has duplicated id with previous "
          << new_display_item_list_[index].AsDebugString().Utf8().data()
          << " (index=" << index << ")";
    }
    AddToIndicesByClientMap(display_item.Client(),
                            new_display_item_list_.size() - 1,
                            new_display_item_indices_by_client_);
  }
#endif   

  if (RuntimeEnabledFeatures::PaintUnderInvalidationCheckingEnabled())
    CheckUnderInvalidation();

  if (!frame_first_paints_.back().first_painted && display_item.IsDrawing() &&
      display_item.GetType() != DisplayItem::kDocumentBackground &&
      display_item.DrawsContent()) {
    SetFirstPainted();
  }
}
