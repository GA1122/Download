void TabStrip::CalculateBoundsForDraggedTabs(const std::vector<Tab*>& tabs,
                                             std::vector<gfx::Rect>* bounds) {
  int x = 0;
  for (size_t i = 0; i < tabs.size(); ++i) {
    Tab* tab = tabs[i];
    if (i > 0 && tab->data().mini != tabs[i - 1]->data().mini)
      x += kMiniToNonMiniGap;
    gfx::Rect new_bounds = tab->bounds();
    new_bounds.set_origin(gfx::Point(x, 0));
    bounds->push_back(new_bounds);
    x += tab->width() + tab_h_offset();
  }
}
