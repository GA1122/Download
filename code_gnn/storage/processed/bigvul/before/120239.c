size_t LayerTreeHost::MaxPartialTextureUpdates() const {
  size_t max_partial_texture_updates = 0;
  if (proxy_->GetRendererCapabilities().allow_partial_texture_updates &&
      !settings_.impl_side_painting) {
    max_partial_texture_updates =
        std::min(settings_.max_partial_texture_updates,
                 proxy_->MaxPartialTextureUpdates());
  }
  return max_partial_texture_updates;
}
