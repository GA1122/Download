gfx::ImageSkia CreateWallpaperImage(SkColor fill, SkColor rect) {
  gfx::Size image_size(1366, 768);
  gfx::Canvas canvas(image_size, 1.0f, true);
  canvas.DrawColor(fill);
  SkPaint paint;
  paint.setColor(rect);
  paint.setStrokeWidth(10);
  paint.setStyle(SkPaint::kStroke_Style);
  paint.setXfermodeMode(SkXfermode::kSrcOver_Mode);
  canvas.DrawRoundRect(gfx::Rect(image_size), 100, paint);
  return gfx::ImageSkia(canvas.ExtractImageRep());
}
