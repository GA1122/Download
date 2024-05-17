gfx::Size WebMediaPlayerImpl::GetCanvasSize() const {
  if (!surface_layer_for_video_enabled_) {
    if (!video_layer_)
      return pipeline_metadata_.natural_size;

    return video_layer_->bounds();
  }
  if (!bridge_->GetCcLayer())
    return pipeline_metadata_.natural_size;

  return bridge_->GetCcLayer()->bounds();
}
