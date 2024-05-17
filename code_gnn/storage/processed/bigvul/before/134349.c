gfx::ImageSkia NewTabButton::GetImageForState(
    views::CustomButton::ButtonState state,
    ui::ScaleFactor scale_factor) const {
  const int overlay_id = state == views::CustomButton::STATE_PRESSED ?
        IDR_NEWTAB_BUTTON_P : IDR_NEWTAB_BUTTON;
  gfx::ImageSkia* overlay = GetThemeProvider()->GetImageSkiaNamed(overlay_id);

  gfx::Canvas canvas(
      gfx::Size(overlay->width(), overlay->height()),
      ui::GetImageScale(scale_factor),
      false);
  canvas.DrawImageInt(GetBackgroundImage(state, scale_factor), 0, 0);

  const int kNativeFrameOverlayAlpha = 178;
  const int kOpaqueFrameOverlayAlpha = 230;
  uint8 alpha = ShouldUseNativeFrame() ?
      kNativeFrameOverlayAlpha : kOpaqueFrameOverlayAlpha;
  canvas.DrawImageInt(*overlay, 0, 0, alpha);

  return gfx::ImageSkia(canvas.ExtractImageRep());
}
