gfx::Size PictureLayerImpl::CalculateTileSize(
    const gfx::Size& content_bounds) const {
  int max_texture_size =
      layer_tree_impl()->resource_provider()->max_texture_size();

  if (is_mask_) {
    if (content_bounds.width() > max_texture_size ||
        content_bounds.height() > max_texture_size)
      return gfx::Size();
    return content_bounds;
  }

  int default_tile_width = 0;
  int default_tile_height = 0;
  if (layer_tree_impl()->use_gpu_rasterization()) {
    int viewport_width = layer_tree_impl()->device_viewport_size().width();
    int viewport_height = layer_tree_impl()->device_viewport_size().height();
    default_tile_width = viewport_width;
    int divisor = 4;
    if (content_bounds.width() <= viewport_width / 2)
      divisor = 2;
    if (content_bounds.width() <= viewport_width / 4)
      divisor = 1;
    default_tile_height = RoundUp(viewport_height, divisor) / divisor;
    default_tile_height += 2 * PictureLayerTiling::kBorderTexels;
    default_tile_height =
        std::max(default_tile_height, kMinHeightForGpuRasteredTile);
  } else {
    const LayerTreeSettings& settings = layer_tree_impl()->settings();
    int max_untiled_content_width = settings.max_untiled_layer_size.width();
    int max_untiled_content_height = settings.max_untiled_layer_size.height();
    default_tile_width = settings.default_tile_size.width();
    default_tile_height = settings.default_tile_size.height();

    if (content_bounds.width() < default_tile_width)
      default_tile_height = max_untiled_content_height;
    if (content_bounds.height() < default_tile_height)
      default_tile_width = max_untiled_content_width;
    if (content_bounds.width() < max_untiled_content_width &&
        content_bounds.height() < max_untiled_content_height) {
      default_tile_height = max_untiled_content_height;
      default_tile_width = max_untiled_content_width;
    }
  }

  int tile_width = default_tile_width;
  int tile_height = default_tile_height;

  if (content_bounds.width() < default_tile_width) {
    tile_width = std::min(tile_width, content_bounds.width());
    tile_width = RoundUp(tile_width, kTileRoundUp);
    tile_width = std::min(tile_width, default_tile_width);
  }
  if (content_bounds.height() < default_tile_height) {
    tile_height = std::min(tile_height, content_bounds.height());
    tile_height = RoundUp(tile_height, kTileRoundUp);
    tile_height = std::min(tile_height, default_tile_height);
  }

  tile_width = std::min(tile_width, max_texture_size);
  tile_height = std::min(tile_height, max_texture_size);
  return gfx::Size(tile_width, tile_height);
}
