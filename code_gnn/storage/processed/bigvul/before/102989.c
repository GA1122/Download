void TabStripModel::MoveTabPrevious() {
  int new_index = std::max(active_index() - 1, 0);
  MoveTabContentsAt(active_index(), new_index, true);
}
