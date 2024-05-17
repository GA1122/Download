bool GLSurfaceOzoneEGL::ScheduleOverlayPlane(int z_order,
                                             OverlayTransform transform,
                                             GLImage* image,
                                             const Rect& bounds_rect,
                                             const RectF& crop_rect) {
  return image->ScheduleOverlayPlane(widget_, z_order, transform, bounds_rect,
                                     crop_rect);
}
