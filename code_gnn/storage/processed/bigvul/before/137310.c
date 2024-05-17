void Textfield::UpdateBackgroundColor() {
  const SkColor color = GetBackgroundColor();
  if (ui::MaterialDesignController::IsSecondaryUiMaterial()) {
    SetBackground(
        CreateBackgroundFromPainter(Painter::CreateSolidRoundRectPainter(
            color, FocusableBorder::kCornerRadiusDp)));
  } else {
    SetBackground(CreateSolidBackground(color));
  }
  GetRenderText()->set_subpixel_rendering_suppressed(SkColorGetA(color) !=
                                                     SK_AlphaOPAQUE);
  SchedulePaint();
}
