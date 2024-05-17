Tab* TabStrip::tab_at(int index) const {
  return static_cast<Tab*>(tabs_.view_at(index));
}
