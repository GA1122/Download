void PaintController::TrackRasterInvalidation(const DisplayItemClient& client,
                                              PaintChunk& chunk,
                                              PaintInvalidationReason reason) {
  DCHECK(raster_invalidation_tracking_info_);

  RasterInvalidationInfo info;
  info.client = &client;
  if (reason == PaintInvalidationReason::kNone) {
    DCHECK(!ClientCacheIsValid(client));
    info.reason = PaintInvalidationReason::kFull;
  } else {
    info.reason = reason;
  }

  if (reason == PaintInvalidationReason::kDisappeared) {
    info.client_debug_name =
        raster_invalidation_tracking_info_->old_client_debug_names.at(&client);
  } else {
    info.client_debug_name = client.DebugName();
  }

  new_paint_chunks_.TrackRasterInvalidation(chunk, info);
}
