void PaintController::AddRasterInvalidation(const DisplayItemClient& client,
                                            PaintChunk& chunk,
                                            const FloatRect& rect,
                                            PaintInvalidationReason reason) {
  new_paint_chunks_.AddRasterInvalidation(chunk, rect);
  if (RasterInvalidationTracking::ShouldAlwaysTrack())
    EnsureRasterInvalidationTracking();
  if (raster_invalidation_tracking_info_)
    TrackRasterInvalidation(client, chunk, reason);
}
