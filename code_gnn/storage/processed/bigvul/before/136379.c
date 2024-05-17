  static const Vector<RasterInvalidationInfo> TrackedRasterInvalidations(
      CompositedLayerRasterInvalidator& invalidator) {
    DCHECK(invalidator.GetTracking());
    return invalidator.GetTracking()->Invalidations();
  }
