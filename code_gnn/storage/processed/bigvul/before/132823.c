PictureLayerImpl::CreatePictureLayerTilingSet() {
  const LayerTreeSettings& settings = layer_tree_impl()->settings();
  return PictureLayerTilingSet::Create(
      this, settings.max_tiles_for_interest_area,
      layer_tree_impl()->use_gpu_rasterization()
          ? 0.f
          : settings.skewport_target_time_in_seconds,
      settings.skewport_extrapolation_limit_in_content_pixels);
}
