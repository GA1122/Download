void PictureLayerImpl::CleanUpTilingsOnActiveLayer(
    std::vector<PictureLayerTiling*> used_tilings) {
  DCHECK(layer_tree_impl()->IsActiveTree());
  if (tilings_->num_tilings() == 0)
    return;

  float min_acceptable_high_res_scale = std::min(
      raster_contents_scale_, ideal_contents_scale_);
  float max_acceptable_high_res_scale = std::max(
      raster_contents_scale_, ideal_contents_scale_);

  PictureLayerImpl* twin = GetPendingOrActiveTwinLayer();
  if (twin && twin->CanHaveTilings()) {
    min_acceptable_high_res_scale = std::min(
        min_acceptable_high_res_scale,
        std::min(twin->raster_contents_scale_, twin->ideal_contents_scale_));
    max_acceptable_high_res_scale = std::max(
        max_acceptable_high_res_scale,
        std::max(twin->raster_contents_scale_, twin->ideal_contents_scale_));
  }

  PictureLayerTilingSet* twin_set = twin ? twin->tilings_.get() : nullptr;
  PictureLayerImpl* recycled_twin = GetRecycledTwinLayer();
  PictureLayerTilingSet* recycled_twin_set =
      recycled_twin ? recycled_twin->tilings_.get() : nullptr;

  tilings_->CleanUpTilings(min_acceptable_high_res_scale,
                           max_acceptable_high_res_scale, used_tilings,
                           layer_tree_impl()->create_low_res_tiling(), twin_set,
                           recycled_twin_set);

  if (recycled_twin_set && recycled_twin_set->num_tilings() == 0)
    recycled_twin->ResetRasterScale();

  DCHECK_GT(tilings_->num_tilings(), 0u);
  SanityCheckTilingState();
}
