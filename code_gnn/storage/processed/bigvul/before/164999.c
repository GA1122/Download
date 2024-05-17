HitTestCanvasResult* HTMLCanvasElement::GetControlAndIdIfHitRegionExists(
    const LayoutPoint& location) {
  if (Is2d())
    return context_->GetControlAndIdIfHitRegionExists(location);
  return HitTestCanvasResult::Create(String(), nullptr);
}
