size_t BackingStoreWin::MemorySize() {
  return size().GetArea() * (color_depth_ / 8);
}
