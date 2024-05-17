static const IntSize MaxDragImageSize(float device_scale_factor) {
#if defined(OS_MACOSX)
  static const IntSize kMaxDragImageSize(400, 400);
#else
  static const IntSize kMaxDragImageSize(200, 200);
#endif
  IntSize max_size_in_pixels = kMaxDragImageSize;
  max_size_in_pixels.Scale(device_scale_factor);
  return max_size_in_pixels;
}
