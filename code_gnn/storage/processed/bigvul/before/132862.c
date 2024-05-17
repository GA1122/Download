void PictureLayerImpl::UpdateTiles(const Occlusion& occlusion_in_content_space,
                                   bool resourceless_software_draw) {
  DCHECK_EQ(1.f, contents_scale_x());
  DCHECK_EQ(1.f, contents_scale_y());

  DoPostCommitInitializationIfNeeded();

  if (!resourceless_software_draw) {
    visible_rect_for_tile_priority_ = visible_content_rect();
  }

  if (!CanHaveTilings()) {
    ideal_page_scale_ = 0.f;
    ideal_device_scale_ = 0.f;
    ideal_contents_scale_ = 0.f;
    ideal_source_scale_ = 0.f;
    SanityCheckTilingState();
    return;
  }

  UpdateIdealScales();

  if (!raster_contents_scale_ || ShouldAdjustRasterScale()) {
    RecalculateRasterScales();
    AddTilingsForRasterScale();
  }

  DCHECK(raster_page_scale_);
  DCHECK(raster_device_scale_);
  DCHECK(raster_source_scale_);
  DCHECK(raster_contents_scale_);
  DCHECK(low_res_raster_contents_scale_);

  was_screen_space_transform_animating_ =
      draw_properties().screen_space_transform_is_animating;

  if (draw_transform_is_animating())
    raster_source_->SetShouldAttemptToUseDistanceFieldText();

  should_update_tile_priorities_ = true;

  UpdateTilePriorities(occlusion_in_content_space);
}
