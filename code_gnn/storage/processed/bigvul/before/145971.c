std::string GetPlacementString(const gfx::Rect& bounds,
                               ui::WindowShowState state) {
  return bounds.ToString() + base::StringPrintf(" %d", state);
}
