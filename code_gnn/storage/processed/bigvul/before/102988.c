void TabStripModel::MoveTabNext() {
  int new_index = std::min(active_index() + 1, count() - 1);
  MoveTabContentsAt(active_index(), new_index, true);
}
