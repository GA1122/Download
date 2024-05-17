double GuestViewBase::PhysicalPixelsToLogicalPixels(int physical_pixels) const {
  DCHECK(physical_pixels >= 0);
  double zoom_factor = GetEmbedderZoomFactor();
  return physical_pixels / zoom_factor;
}
