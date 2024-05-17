void BaseRenderingContext2D::CheckOverdraw(
    const SkRect& rect,
    const PaintFlags* flags,
    CanvasRenderingContext2DState::ImageType image_type,
    DrawType draw_type) {
  PaintCanvas* c = DrawingCanvas();
  if (!c)
    return;

  SkRect device_rect;
  if (draw_type == kUntransformedUnclippedFill) {
    device_rect = rect;
  } else {
    DCHECK_EQ(draw_type, kClipFill);
    if (GetState().HasComplexClip())
      return;

    SkIRect sk_i_bounds;
    if (!c->getDeviceClipBounds(&sk_i_bounds))
      return;
    device_rect = SkRect::Make(sk_i_bounds);
  }

  const SkImageInfo& image_info = c->imageInfo();
  if (!device_rect.contains(
          SkRect::MakeWH(image_info.width(), image_info.height())))
    return;

  bool is_source_over = true;
  unsigned alpha = 0xFF;
  if (flags) {
    if (flags->getLooper() || flags->getImageFilter() || flags->getMaskFilter())
      return;

    SkBlendMode mode = flags->getBlendMode();
    is_source_over = mode == SkBlendMode::kSrcOver;
    if (!is_source_over && mode != SkBlendMode::kSrc &&
        mode != SkBlendMode::kClear)
      return;   

    alpha = flags->getAlpha();

    if (is_source_over &&
        image_type == CanvasRenderingContext2DState::kNoImage) {
      if (flags->HasShader()) {
        if (flags->ShaderIsOpaque() && alpha == 0xFF)
          this->WillOverwriteCanvas();
        return;
      }
    }
  }

  if (is_source_over) {
    if (image_type == CanvasRenderingContext2DState::kNonOpaqueImage)
      return;
    if (alpha < 0xFF)
      return;
  }

  this->WillOverwriteCanvas();
}
