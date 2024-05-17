  VoiceInteractionIconBackground()
      : Layer(ui::LAYER_NOT_DRAWN),
        large_size_(
            gfx::Size(kBackgroundLargeWidthDip, kBackgroundLargeHeightDip)),
        small_size_(gfx::Size(kBackgroundSizeDip, kBackgroundSizeDip)),
        center_point_(
            gfx::PointF(kBackgroundSizeDip / 2, kBackgroundSizeDip / 2)),
        circle_layer_delegate_(std::make_unique<views::CircleLayerDelegate>(
            SK_ColorWHITE,
            kBackgroundSizeDip / 2)),
        rect_layer_delegate_(std::make_unique<views::RectangleLayerDelegate>(
            SK_ColorWHITE,
            gfx::SizeF(small_size_))) {
    set_name("VoiceInteractionOverlay:BACKGROUND_LAYER");
    SetBounds(gfx::Rect(0, 0, kBackgroundInitSizeDip, kBackgroundInitSizeDip));
    SetFillsBoundsOpaquely(false);
    SetMasksToBounds(false);

    shadow_values_ =
        gfx::ShadowValue::MakeMdShadowValues(kBackgroundShadowElevationDip);
    const gfx::Insets shadow_margin =
        gfx::ShadowValue::GetMargin(shadow_values_);

    border_shadow_delegate_ =
        std::make_unique<views::BorderShadowLayerDelegate>(
            shadow_values_, gfx::Rect(large_size_), SK_ColorWHITE,
            kBackgroundCornerRadiusDip);

    large_shadow_layer_ = std::make_unique<ui::Layer>();
    large_shadow_layer_->set_delegate(border_shadow_delegate_.get());
    large_shadow_layer_->SetFillsBoundsOpaquely(false);
    large_shadow_layer_->SetBounds(
        gfx::Rect(shadow_margin.left(), shadow_margin.top(),
                  kBackgroundLargeWidthDip - shadow_margin.width(),
                  kBackgroundLargeHeightDip - shadow_margin.height()));
    Add(large_shadow_layer_.get());

    shadow_layer_ = std::make_unique<ui::Layer>();
    shadow_layer_->set_delegate(this);
    shadow_layer_->SetFillsBoundsOpaquely(false);
    shadow_layer_->SetBounds(
        gfx::Rect(shadow_margin.left(), shadow_margin.top(),
                  kBackgroundInitSizeDip - shadow_margin.width(),
                  kBackgroundInitSizeDip - shadow_margin.height()));
    Add(shadow_layer_.get());

    for (int i = 0; i < PAINTED_SHAPE_COUNT; ++i)
      AddPaintLayer(static_cast<PaintedShape>(i));
  }
