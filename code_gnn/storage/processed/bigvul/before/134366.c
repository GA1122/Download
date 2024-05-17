bool TabStrip::IsActiveDropTarget() const {
  for (int i = 0; i < tab_count(); ++i) {
    Tab* tab = tab_at(i);
    if (tab->dragging())
      return true;
  }
  return false;
}
