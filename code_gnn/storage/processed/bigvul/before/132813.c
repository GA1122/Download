bool PictureLayerImpl::AllTilesRequiredAreReadyToDraw(
    TileRequirementCheck is_tile_required_callback) const {
  if (!HasValidTilePriorities())
    return true;

  if (!tilings_)
    return true;

  if (visible_rect_for_tile_priority_.IsEmpty())
    return true;

  gfx::Rect rect = GetViewportForTilePriorityInContentSpace();
  rect.Intersect(visible_rect_for_tile_priority_);

  PictureLayerTiling* tiling =
      tilings_->FindTilingWithResolution(HIGH_RESOLUTION);
  if (!tiling)
    return true;

  for (PictureLayerTiling::CoverageIterator iter(tiling, 1.f, rect); iter;
       ++iter) {
    const Tile* tile = *iter;
    if (!tile)
      continue;

    if ((tiling->*is_tile_required_callback)(tile) && !tile->IsReadyToDraw()) {
      TRACE_EVENT_INSTANT0("cc", "Tile required, but not ready to draw.",
                           TRACE_EVENT_SCOPE_THREAD);
      return false;
    }
  }

  return true;
}
