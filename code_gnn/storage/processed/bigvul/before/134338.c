gfx::ImageSkia NewTabButton::GetBackgroundImage(
    views::CustomButton::ButtonState state,
    ui::ScaleFactor scale_factor) const {
  int background_id = 0;
  if (ShouldUseNativeFrame()) {
    background_id = IDR_THEME_TAB_BACKGROUND_V;
  } else if (tab_strip_->controller()->IsIncognito()) {
    background_id = IDR_THEME_TAB_BACKGROUND_INCOGNITO;
#if defined(OS_WIN)
  } else if (win8::IsSingleWindowMetroMode()) {
    background_id = IDR_THEME_TAB_BACKGROUND_V;
#endif
  } else {
    background_id = IDR_THEME_TAB_BACKGROUND;
  }

  int alpha = 0;
  switch (state) {
    case views::CustomButton::STATE_NORMAL:
    case views::CustomButton::STATE_HOVERED:
      alpha = ShouldUseNativeFrame() ? kNativeFrameInactiveTabAlpha : 255;
      break;
    case views::CustomButton::STATE_PRESSED:
      alpha = 145;
      break;
    default:
      NOTREACHED();
      break;
  }

  gfx::ImageSkia* mask =
      GetThemeProvider()->GetImageSkiaNamed(IDR_NEWTAB_BUTTON_MASK);
  int height = mask->height();
  int width = mask->width();
  float scale = ui::GetImageScale(scale_factor);
  if (!mask->HasRepresentation(scale))
    scale_factor = ui::SCALE_FACTOR_100P;

  gfx::Canvas canvas(gfx::Size(width, height), scale, false);

  gfx::ImageSkia* background =
      GetThemeProvider()->GetImageSkiaNamed(background_id);
  int offset_y = GetThemeProvider()->HasCustomImage(background_id) ?
      0 : background_offset_.y();

  float x_scale = 1.0f;
  int x = GetMirroredX() + background_offset_.x();
  if (base::i18n::IsRTL()) {
    x_scale = -1.0f;
    x += width;
  }
  canvas.TileImageInt(*background, x, newtab_button_v_offset() + offset_y,
                      x_scale, 1.0f, 0, 0, width, height);

  if (alpha != 255) {
    SkPaint paint;
    paint.setColor(SkColorSetARGB(alpha, 255, 255, 255));
    paint.setXfermodeMode(SkXfermode::kDstIn_Mode);
    paint.setStyle(SkPaint::kFill_Style);
    canvas.DrawRect(gfx::Rect(0, 0, width, height), paint);
  }

  if (state == views::CustomButton::STATE_HOVERED)
    canvas.FillRect(GetLocalBounds(), SkColorSetARGB(64, 255, 255, 255));

  return gfx::ImageSkiaOperations::CreateMaskedImage(
      gfx::ImageSkia(canvas.ExtractImageRep()), *mask);
}
