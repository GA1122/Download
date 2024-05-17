void CompositedLayerRasterInvalidator::FullyInvalidateOldChunk(
    const PaintChunkInfo& info,
    PaintInvalidationReason reason) {
  String debug_name;
  if (tracking_info_)
    debug_name = tracking_info_->old_client_debug_names.at(&info.id.client);
  AddRasterInvalidation(info.bounds_in_layer, &info.id.client, reason,
                        &debug_name);
}
