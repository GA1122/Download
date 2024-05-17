bool TabStripModel::ContainsIndex(int index) const {
  return index >= 0 && index < count();
}
