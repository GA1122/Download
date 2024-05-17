bool GLSurfaceOzoneSurfaceless::ScheduleOverlayPlane(int z_order,
                                                     OverlayTransform transform,
                                                     GLImage* image,
                                                     const Rect& bounds_rect,
                                                     const RectF& crop_rect) {
  unsubmitted_frames_.back()->overlays.push_back(
      GLSurfaceOverlay(z_order, transform, image, bounds_rect, crop_rect));
  return true;
}
