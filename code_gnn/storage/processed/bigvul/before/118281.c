void AutofillDialogViews::OverlayView::OnPaint(gfx::Canvas* canvas) {
  gfx::Rect rect = ContentBoundsSansBubbleBorder();
  const SkScalar kCornerRadius = SkIntToScalar(
      GetBubbleBorder() ? GetBubbleBorder()->GetBorderCornerRadius() : 2);
  gfx::Path window_mask;
  window_mask.addRoundRect(gfx::RectToSkRect(rect),
                           kCornerRadius, kCornerRadius);
  canvas->ClipPath(window_mask, false);

  OnPaintBackground(canvas);

  if (message_view_->visible()) {
    const int arrow_half_width = kArrowWidth / 2.0f;
    SkPath arrow;
    int y = message_view_->y() - 1;
    arrow.moveTo(rect.x() - 1, y);
    arrow.rLineTo(rect.width() / 2 - arrow_half_width, 0);
    arrow.rLineTo(arrow_half_width, -kArrowHeight);
    arrow.rLineTo(arrow_half_width, kArrowHeight);
    arrow.lineTo(rect.right() + 1, y);
    arrow.lineTo(rect.right() + 1, rect.bottom() + 1);
    arrow.lineTo(rect.x() - 1, rect.bottom() + 1);
    arrow.close();

    SkPaint paint;
    SkColor background_color = background()->get_color();
    int background_luminance =
        color_utils::GetLuminanceForColor(background_color);
    int background_alpha = static_cast<int>(
        7 + 15 * (255 - background_luminance) / 255);
    int subtle_border_alpha = static_cast<int>(
        10 + 20 * (255 - background_luminance) / 255);

    paint.setColor(color_utils::BlendTowardOppositeLuminance(
        background_color, background_alpha));
    paint.setStyle(SkPaint::kFill_Style);
    canvas->DrawPath(arrow, paint);
    paint.setColor(color_utils::BlendTowardOppositeLuminance(
        background_color, subtle_border_alpha));
    paint.setStyle(SkPaint::kStroke_Style);
    canvas->DrawPath(arrow, paint);
  }
}
