void HTMLCanvasElement::DidDraw(const FloatRect& rect) {
  if (rect.IsEmpty())
    return;
  canvas_is_clear_ = false;
  if (GetLayoutObject() && !LowLatencyEnabled())
    GetLayoutObject()->SetShouldCheckForPaintInvalidation();
  if (Is2d() && context_->ShouldAntialias() && GetPage() &&
      GetPage()->DeviceScaleFactorDeprecated() > 1.0f) {
    FloatRect inflated_rect = rect;
    inflated_rect.Inflate(1);
    dirty_rect_.Unite(inflated_rect);
  } else {
    dirty_rect_.Unite(rect);
  }
  if (Is2d() && canvas2d_bridge_)
    canvas2d_bridge_->DidDraw(rect);
}
