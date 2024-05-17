RasterInvalidationTracking& CompositedLayerRasterInvalidator::EnsureTracking() {
  if (!tracking_info_)
    tracking_info_ = std::make_unique<RasterInvalidationTrackingInfo>();
  return tracking_info_->tracking;
}
