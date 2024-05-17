void SkiaOutputSurfaceImpl::SetDisplayTransformHint(
    gfx::OverlayTransform transform) {
  if (capabilities_.supports_pre_transform)
    pre_transform_ = transform;
}
