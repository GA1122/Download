void CompositedLayerRasterInvalidator::SetTracksRasterInvalidations(
    bool should_track) {
  if (should_track) {
    if (!tracking_info_)
      tracking_info_ = std::make_unique<RasterInvalidationTrackingInfo>();
    tracking_info_->tracking.ClearInvalidations();
    for (const auto& info : paint_chunks_info_) {
      tracking_info_->old_client_debug_names.Set(&info.id.client,
                                                 info.id.client.DebugName());
    }
  } else if (!RasterInvalidationTracking::ShouldAlwaysTrack()) {
    tracking_info_ = nullptr;
  } else if (tracking_info_) {
    tracking_info_->tracking.ClearInvalidations();
  }
}
