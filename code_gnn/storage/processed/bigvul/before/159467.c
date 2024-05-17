bool FrameIsValid(const SkBitmap& frame_bitmap) {
  return !frame_bitmap.isNull() && !frame_bitmap.empty() &&
         frame_bitmap.colorType() == kN32_SkColorType;
}
