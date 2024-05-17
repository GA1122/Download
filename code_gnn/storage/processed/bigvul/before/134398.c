void NewTabButton::OnPaint(gfx::Canvas* canvas) {
  gfx::ImageSkia image =
      GetImageForScale(ui::GetSupportedScaleFactor(canvas->image_scale()));
  canvas->DrawImageInt(image, 0, height() - image.height());
}
