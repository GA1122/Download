void PictureLayer::SetLayerTreeHost(LayerTreeHost* host) {
  Layer::SetLayerTreeHost(host);
  if (host) {
    if (!recording_source_) {
      if (host->settings().use_display_lists) {
        recording_source_.reset(new DisplayListRecordingSource);
      } else {
        recording_source_.reset(new PicturePile);
      }
    }
    recording_source_->SetMinContentsScale(
        host->settings().minimum_contents_scale);
    recording_source_->SetTileGridSize(host->settings().default_tile_grid_size);
    recording_source_->SetSlowdownRasterScaleFactor(
        host->debug_state().slow_down_raster_scale_factor);
  }
}
