PaintLayerScrollableArea::GetCompositorAnimationTimeline() const {
  return layer_->GetLayoutObject()
      .GetFrameView()
      ->GetCompositorAnimationTimeline();
}
