void CompositedLayerRasterInvalidator::AddRasterInvalidation(
    const IntRect& rect,
    const DisplayItemClient* client,
    PaintInvalidationReason reason,
    const String* debug_name) {
  raster_invalidation_function_(rect);
  if (tracking_info_) {
    tracking_info_->tracking.AddInvalidation(
        client, debug_name ? *debug_name : client->DebugName(), rect, reason);
  }
}
