void TabStrip::SetIdealBoundsFromPositions(const std::vector<int>& positions) {
  if (static_cast<size_t>(tab_count()) != positions.size())
    return;

  for (int i = 0; i < tab_count(); ++i) {
    gfx::Rect bounds(ideal_bounds(i));
    bounds.set_x(positions[i]);
    set_ideal_bounds(i, bounds);
  }
}
