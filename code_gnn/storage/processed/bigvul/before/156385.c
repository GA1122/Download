gfx::Size DetermineSnapshotSize(const gfx::Size& surface_size,
                                int screencast_max_width,
                                int screencast_max_height) {
  if (surface_size.IsEmpty())
    return gfx::Size();   

  double scale = 1;
  if (screencast_max_width > 0) {
    scale = std::min(scale, static_cast<double>(screencast_max_width) /
                                surface_size.width());
  }
  if (screencast_max_height > 0) {
    scale = std::min(scale, static_cast<double>(screencast_max_height) /
                                surface_size.height());
  }
  return gfx::ToRoundedSize(gfx::ScaleSize(gfx::SizeF(surface_size), scale));
}
