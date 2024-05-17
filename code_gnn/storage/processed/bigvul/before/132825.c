scoped_refptr<Tile> PictureLayerImpl::CreateTile(PictureLayerTiling* tiling,
                                               const gfx::Rect& content_rect) {
  DCHECK(!raster_source_->IsSolidColor());
  if (!raster_source_->CoversRect(content_rect, tiling->contents_scale()))
    return scoped_refptr<Tile>();

  int flags = 0;

  if (!is_mask_)
    flags = Tile::USE_PICTURE_ANALYSIS;

  return layer_tree_impl()->tile_manager()->CreateTile(
      raster_source_.get(), content_rect.size(), content_rect,
      tiling->contents_scale(), id(), layer_tree_impl()->source_frame_number(),
      flags);
}
