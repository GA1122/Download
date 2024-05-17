void Instance::FillRect(const pp::Rect& rect, uint32 color) {
  DCHECK(!image_data_.is_null() || rect.IsEmpty());
  uint32* buffer_start = static_cast<uint32*>(image_data_.data());
  int stride = image_data_.stride();
  uint32* ptr = buffer_start + rect.y() * stride / 4 + rect.x();
  int height = rect.height();
  int width = rect.width();
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x)
      *(ptr + x) = color;
    ptr += stride / 4;
  }
}
