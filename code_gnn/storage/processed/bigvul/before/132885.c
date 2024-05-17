  void SetupPendingTreeWithInvalidation(
      scoped_refptr<RasterSource> raster_source,
      const Region& invalidation) {
    SetupPendingTreeInternal(raster_source, gfx::Size(), invalidation);
  }
