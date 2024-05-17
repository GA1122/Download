XcursorImage* SkBitmapToXcursorImage(const SkBitmap* cursor_image,
                                     const gfx::Point& hotspot) {
  DCHECK(cursor_image->config() == SkBitmap::kARGB_8888_Config);
  gfx::Point hotspot_point = hotspot;
  SkBitmap scaled;

  const float kMaxPixel = 64.f;
  bool needs_scale = false;
  if (cursor_image->width() > kMaxPixel || cursor_image->height() > kMaxPixel) {
    float scale = 1.f;
    if (cursor_image->width() > cursor_image->height())
      scale = kMaxPixel / cursor_image->width();
    else
      scale = kMaxPixel / cursor_image->height();

    scaled = skia::ImageOperations::Resize(*cursor_image,
        skia::ImageOperations::RESIZE_BETTER,
        static_cast<int>(cursor_image->width() * scale),
        static_cast<int>(cursor_image->height() * scale));
    hotspot_point = hotspot.Scale(scale);
    needs_scale = true;
  }

  const SkBitmap* bitmap = needs_scale ? &scaled : cursor_image;
  XcursorImage* image = XcursorImageCreate(bitmap->width(), bitmap->height());
  image->xhot = hotspot_point.x();
  image->yhot = hotspot_point.y();

  if (bitmap->width() && bitmap->height()) {
    bitmap->lockPixels();
    memcpy(image->pixels,
           bitmap->getPixels(),
           bitmap->width() * bitmap->height() * 4);
    bitmap->unlockPixels();
  }

  return image;
}
