void PaintController::InvalidateAll() {
  DCHECK(!RuntimeEnabledFeatures::SlimmingPaintV2Enabled());
  InvalidateAllInternal();
}
