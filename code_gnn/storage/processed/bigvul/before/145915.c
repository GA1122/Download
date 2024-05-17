display::ManagedDisplayInfo CreateDisplayInfo(int64_t id,
                                              const gfx::Rect& bounds) {
  display::ManagedDisplayInfo info(id, ToDisplayName(id), false);
  info.SetBounds(bounds);
  return info;
}
