void TabStrip::PaintClosingTabs(gfx::Canvas* canvas, int index) {
  if (tabs_closing_map_.find(index) == tabs_closing_map_.end())
    return;

  const std::vector<Tab*>& tabs = tabs_closing_map_[index];
  for (std::vector<Tab*>::const_reverse_iterator i(tabs.rbegin());
       i != tabs.rend(); ++i) {
    (*i)->Paint(canvas);
  }
}
