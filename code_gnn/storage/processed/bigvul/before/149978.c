void LayerTreeHostImpl::NotifyAllTileTasksCompleted() {
  if (global_tile_state_.hard_memory_limit_in_bytes == 0) {
    if (image_decode_cache_)
      image_decode_cache_->SetShouldAggressivelyFreeResources(true);
    SetContextVisibility(false);
  }
}
