void TabStrip::AnimateToIdealBounds() {
  for (int i = 0; i < tab_count(); ++i) {
    Tab* tab = tab_at(i);
    if (!tab->dragging())
      bounds_animator_.AnimateViewTo(tab, ideal_bounds(i));
  }

  bounds_animator_.AnimateViewTo(newtab_button_, newtab_button_bounds_);
}
