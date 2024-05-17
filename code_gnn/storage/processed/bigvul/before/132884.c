  void SetupPendingTreeWithFixedTileSize(
      scoped_refptr<RasterSource> raster_source,
      const gfx::Size& tile_size,
      const Region& invalidation) {
    SetupPendingTreeInternal(raster_source, tile_size, invalidation);
  }
