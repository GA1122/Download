scoped_refptr<Image> HTMLCanvasElement::GetSourceImageForCanvas(
    SourceImageStatus* status,
    AccelerationHint hint,
    const FloatSize&) {
  if (!width() || !height()) {
    *status = kZeroSizeCanvasSourceImageStatus;
    return nullptr;
  }

  if (!IsPaintable()) {
    *status = kInvalidSourceImageStatus;
    return nullptr;
  }

  if (PlaceholderFrame()) {
    *status = kNormalSourceImageStatus;
    return PlaceholderFrame()->Bitmap();
  }

  if (!context_) {
    scoped_refptr<Image> result = CreateTransparentImage(Size());
    *status = result ? kNormalSourceImageStatus : kInvalidSourceImageStatus;
    return result;
  }

  if (HasImageBitmapContext()) {
    *status = kNormalSourceImageStatus;
    scoped_refptr<Image> result = context_->GetImage(hint);
    if (!result)
      result = CreateTransparentImage(Size());
    *status = result ? kNormalSourceImageStatus : kInvalidSourceImageStatus;
    return result;
  }

  scoped_refptr<Image> image;
  if (Is3d()) {
    RenderingContext()->PaintRenderingResultsToCanvas(kBackBuffer);
    if (ResourceProvider())
      image = ResourceProvider()->Snapshot();
    else
      image = CreateTransparentImage(Size());
  } else {
    if (canvas_heuristic_parameters::kDisableAccelerationToAvoidReadbacks &&
        !RuntimeEnabledFeatures::Canvas2dFixedRenderingModeEnabled() &&
        hint == kPreferNoAcceleration && canvas2d_bridge_ &&
        canvas2d_bridge_->IsAccelerated() &&
        !base::FeatureList::IsEnabled(features::kAlwaysAccelerateCanvas)) {
      DisableAcceleration();
    }
    image = RenderingContext()->GetImage(hint);
    if (!image)
      image = CreateTransparentImage(Size());
  }

  if (image)
    *status = kNormalSourceImageStatus;
  else
    *status = kInvalidSourceImageStatus;
  return image;
}
