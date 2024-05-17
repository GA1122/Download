void PaintController::SetTracksRasterInvalidations(bool value) {
  if (value) {
    raster_invalidation_tracking_info_ =
        std::make_unique<RasterInvalidationTrackingInfo>();

    DCHECK(new_display_item_list_.IsEmpty());
    for (const auto& item : current_paint_artifact_.GetDisplayItemList()) {
      raster_invalidation_tracking_info_->old_client_debug_names.Set(
          &item.Client(), item.Client().DebugName());
    }
  } else if (!RasterInvalidationTracking::ShouldAlwaysTrack()) {
    raster_invalidation_tracking_info_ = nullptr;
  }
}
