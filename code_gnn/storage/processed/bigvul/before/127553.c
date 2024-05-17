size_t BackingStoreGtk::MemorySize() {
  if (!use_render_)
    return size().GetArea() * (pixmap_bpp_ / 8);
  else
    return size().GetArea() * 4;
}
