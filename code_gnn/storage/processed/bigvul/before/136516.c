void PaintController::CommitNewDisplayItems() {
  TRACE_EVENT2("blink,benchmark", "PaintController::commitNewDisplayItems",
               "current_display_list_size",
               (int)current_paint_artifact_.GetDisplayItemList().size(),
               "num_non_cached_new_items",
               (int)new_display_item_list_.size() - num_cached_new_items_);

  num_cached_new_items_ = 0;
#if DCHECK_IS_ON()
  new_display_item_indices_by_client_.clear();
  new_paint_chunk_indices_by_client_.clear();
#endif

  if (RuntimeEnabledFeatures::SlimmingPaintV175Enabled() &&
      !new_display_item_list_.IsEmpty())
    GenerateRasterInvalidations(new_paint_chunks_.LastChunk());

  auto old_cache_generation = current_cache_generation_;
  current_cache_generation_ =
      DisplayItemClient::CacheGenerationOrInvalidationReason::Next();

  new_cached_subsequences_.swap(current_cached_subsequences_);
  new_cached_subsequences_.clear();
  last_cached_subsequence_end_ = 0;
  for (auto& item : current_cached_subsequences_)
    item.key->SetDisplayItemsCached(current_cache_generation_);

  Vector<const DisplayItemClient*> skipped_cache_clients;
  for (const auto& item : new_display_item_list_) {
    const auto& client = item.Client();
    if (RuntimeEnabledFeatures::SlimmingPaintV175Enabled())
      client.ClearPartialInvalidationRect();

    if (item.IsCacheable()) {
      client.SetDisplayItemsCached(current_cache_generation_);
    } else {
      if (client.IsJustCreated())
        client.ClearIsJustCreated();
      if (item.SkippedCache())
        skipped_cache_clients.push_back(&item.Client());
    }
  }

  for (auto* client : skipped_cache_clients) {
    if (client->DisplayItemsAreCached(old_cache_generation) ||
        client->DisplayItemsAreCached(current_cache_generation_))
      client->SetDisplayItemsUncached();
  }

  new_display_item_list_.ShrinkToFit();

  current_paint_artifact_ = PaintArtifact(std::move(new_display_item_list_),
                                          new_paint_chunks_.ReleaseData());

  ResetCurrentListIndices();
  out_of_order_item_indices_.clear();
  out_of_order_chunk_indices_.clear();
  items_moved_into_new_list_.clear();

  if (RuntimeEnabledFeatures::SlimmingPaintV175Enabled()) {
    for (const auto& chunk : current_paint_artifact_.PaintChunks()) {
      if (chunk.id.client.IsJustCreated())
        chunk.id.client.ClearIsJustCreated();
    }
  }

  new_display_item_list_ = DisplayItemList(0);

#if DCHECK_IS_ON()
  num_sequential_matches_ = 0;
  num_out_of_order_matches_ = 0;
  num_indexed_items_ = 0;
#endif

  if (raster_invalidation_tracking_info_) {
    raster_invalidation_tracking_info_->old_client_debug_names.clear();
    std::swap(raster_invalidation_tracking_info_->old_client_debug_names,
              raster_invalidation_tracking_info_->new_client_debug_names);
  }

#if DCHECK_IS_ON()
  if (VLOG_IS_ON(2)) {
    LOG(ERROR) << "PaintController::CommitNewDisplayItems() done";
    if (VLOG_IS_ON(3))
      ShowDebugDataWithRecords();
    else
      ShowDebugData();
  }
#endif
}
